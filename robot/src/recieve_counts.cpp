/*
  recieve_counts TF and Odom topic publisher
  Copyright (c) 2016 Kartik Madhira.  All right reserved.

  This node takes encoder counts coming from Arduino in a custom 
  msg under ticks.h and via the ros subscriber callback function
  and passes the data to the TF odom->base_link transform as well
  as publishes the nav_msgs/Odometery data.
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "ros/ros.h"
#include "ros/time.h"
#include "tf/tf.h"
#include "tf/transform_broadcaster.h"
#include "tf/transform_listener.h"
#include "std_msgs/Float32.h"
#include "robot/ticks.h"
#include "nav_msgs/Odometry.h"
#define DIST_CONST  .3454  //3.14*.11 meters(pi*dia)
#define BASE_WIDTH .32    //30 cm or .30 m
#define Radians         3.14/180


//making global variables so that call back function uses
double x,y,th,delta_x,delta_th,delta_y,X_vel,global_vx,global_vy,ang_v,left_v,right_v,delta_enc,left_wheel,current_time_enc1,current_time_enc2,delta_enc1,delta_enc2;
int32_t right_wheel,current_time_enc,prev_time_enc1,prev_time_enc2;
//creating ros time objects for encoder delta
//creating a ros std_msgs type
robot::ticks data;
void odom_callBack(const robot::ticks &ard_dat );
int main (int argc, char **argv)
{
  //initializing the ros node
  ros::init(argc,argv, "odometry_publisher");
  ros::NodeHandle n;
  ros::Subscriber sub=n.subscribe("/ticks",10, &odom_callBack);
  ros::spinOnce();

  //initializing a subscriber for thr encoder ticks
  //initializing the publisher to write the odom data to nav_msgs/odom topic
  ros::Publisher odom_pub= n.advertise<nav_msgs::Odometry>("odom",10);
  //while getting the odometery data we also need to transform frames
  ros::Rate loop_rate(25);
  ros::spinOnce();

  tf::TransformBroadcaster odom_broad;
  ros::Time current_time, last_time;
  current_time = ros::Time::now();
  last_time = ros::Time::now();

  while(n.ok())
  {     /***********************************
         *            ODOMETERY SECTION            *
         ***********************************/
        current_time_enc1=-data.left_wheel;
        current_time_enc2=-data.right_wheel;
    //getting current_time for the current timing
        current_time = ros::Time::now();
        //getting dt evaluated in seconds
        delta_enc1=current_time_enc1-prev_time_enc1;
        delta_enc2=current_time_enc2-prev_time_enc2;

        double delta=(current_time-last_time).toSec();

        //getting the left and right wheel velocities
        left_v=(delta_enc1 *  DIST_CONST)/(560 * delta);
        right_v=(delta_enc2 * DIST_CONST)/(560 * delta);

        X_vel=(left_v+right_v)/2;
        //getting global vel values
        global_vx=X_vel * cos(th);
        global_vy=X_vel * sin(th);
        ang_v=(left_v-right_v)/BASE_WIDTH;

        delta_x=X_vel * cos(th) * delta;
        delta_y=X_vel * sin(th)* delta;
        delta_th=ang_v * delta;

        x+=delta_x;
        y+=delta_y;
        th-=delta_th;
            /***********************************
            *            TF SECTION            *
            ***********************************/
        //from the odom tuts, we need to convert theta to yaw
        geometry_msgs::Quaternion odom_quat=tf::createQuaternionMsgFromYaw(th);

        //we'll transform the odom frame to base_link at specific times
        geometry_msgs::TransformStamped odom_trans;
        odom_trans.header.stamp=current_time;
        odom_trans.header.frame_id="odom";
        odom_trans.child_frame_id="base_link";

        odom_trans.transform.translation.x=x;
        odom_trans.transform.translation.y=y;
        odom_trans.transform.translation.z=0.0;
        odom_trans.transform.rotation = odom_quat;

        //sending the above transform over ROS
        odom_broad.sendTransform(odom_trans);

        /**********************************W*
                 ODOMTERY SECTION  return 0;

         ***********************************/

        //creating the nav_msgs object to recieve odom message
        nav_msgs::Odometry odom;
        odom.header.stamp = current_time;
        odom.header.frame_id="odom";

        //setting all the data coming from arduino
        odom.pose.pose.position.x=x;
        odom.pose.pose.position.y=y;
        odom.pose.pose.position.z=0.0;
        odom.pose.pose.orientation=odom_quat;

        //setting the velocites from arduino
        odom.child_frame_id="base_link";
        odom.twist.twist.linear.x=global_vx;
        odom.twist.twist.linear.y=0;
        odom.twist.twist.angular.z=ang_v;


        //publishing the message over ROS using the publisher object
        odom_pub.publish(odom);
        last_time=current_time;
        prev_time_enc1=current_time_enc1;
        prev_time_enc2=current_time_enc2;
        ROS_INFO("The left, right and angular velocities are %.2f, %.2f, %.2f:", -data.left_wheel, -data.right_wheel, th);
        ros::spinOnce();
	        loop_rate.sleep();
  }
  return 0;
}

void odom_callBack(const robot::ticks& ard_data)
{ //getting all the odom topic publishing
  data=ard_data;

  //we need to pass all the values and publish the dat}
}
