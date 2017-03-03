/*

/**
  *****************************************************************************
  * @project ticks_send
  * @author  Kartik Madhira <kartikmadhira1@gmail.com>
  * @date    28 Dec 2016
  * @brief   Sending encoder ticks using the Encoder library from pjrc and arduino_rosserial

  */
#define kp 1
#define ki 0
#define kd .4
#define kp1 1
#define ki1 0
#define kd1 .4

#include <Encoder.h>
#include <ros.h>
#include <robot/ticks.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>

//initializing with the help of a nodehandle. This will take care of
//all the node initializations.
float linear_vel,ang_vel,error,act_lin_vel,act_ang_vel,integral,prev_error,final_cmd_vel,error_ang,int_ang,prev_ang_vel,final_ang_vel;
long left_rpm,right_rpm;
void twist(const geometry_msgs::Twist &twist);
void odom_data(const nav_msgs::Odometry &odom_data);
int command();
ros::NodeHandle n;
//since the odom data has to published we use a tf object to send 
//transform of base_link->mobile_base to the ROS core
//since we are going to publish both left and right and right encoder ticks
//using two data types of float32
robot::ticks data;
//creating publisher initializers on topic 
ros::Publisher ard_data1("ticks", &data);
ros::Subscriber<geometry_msgs::Twist> twist1("cmd_vel",twist);
ros::Subscriber<nav_msgs::Odometry> odom1("odom",odom_data);
Encoder leftWheel(2,3);
Encoder rightWheel(19, 18);
volatile long enc_count;

void setup() 
{

pinMode(11, OUTPUT);
pinMode(10, OUTPUT);
pinMode(8, OUTPUT);
pinMode(9, OUTPUT);
pinMode(7, OUTPUT);
pinMode(6, OUTPUT);

n.initNode();
n.advertise(ard_data1);
n.subscribe(twist1);
n.subscribe(odom1);

}


void loop() {

digitalWrite(7, HIGH);
digitalWrite(6, LOW);
digitalWrite(10, HIGH);
digitalWrite(11, LOW);

//pid loop
error=linear_vel-act_lin_vel;
error_ang=ang_vel-act_ang_vel;
integral+=error;
final_cmd_vel=kp*error + kd*(error-prev_error) + ki*(integral);
final_ang_vel=kp1*error_ang + kd1*(error_ang-prev_ang_vel);
command();
analogWrite(8,right_rpm);
analogWrite(9,left_rpm);
data.right_wheel=rightWheel.read();
data.left_wheel=leftWheel.read();
n.spinOnce();
ard_data1.publish(&data);
prev_error=error;
prev_ang_vel=error_ang;
}
void twist(const geometry_msgs::Twist &twist)
{
  linear_vel=twist.linear.x;
  ang_vel=twist.angular.z;
  }

void odom_data(const nav_msgs::Odometry &odom_data)
{
  
  act_lin_vel=odom_data.twist.twist.linear.x;
  act_ang_vel=odom_data.twist.twist.angular.z;
}

int command()
{
  double linear_vel_mins = final_cmd_vel * 60;
  double angular_vel_mins = ang_vel * 60;
  double circumference = PI * 0.11;
  double tangential_vel = angular_vel_mins * 0.30;
  left_rpm=(linear_vel_mins / circumference) + (tangential_vel / circumference);
  right_rpm=(linear_vel_mins / circumference) - (tangential_vel / circumference);
   if(left_rpm<0){
   digitalWrite(10, HIGH);
   digitalWrite(11, HIGH);    
  }
  if(right_rpm<0){
    digitalWrite(7, HIGH);
    digitalWrite(6, HIGH);
  }
  return left_rpm, right_rpm;
}

