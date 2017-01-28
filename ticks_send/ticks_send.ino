/*

/**
  *****************************************************************************
  * @project ticks_send
  * @author  Kartik Madhira <kartikmadhira1@gmail.com>
  * @date    28 Dec 2016
  * @brief   Sending encoder ticks using the Encoder library from pjrc and arduino_rosserial

  */


#include <Encoder.h>
#include <ros.h>
#include <robot/ticks.h>
#include <geometry_msgs/Twist.h>
//initializing with the help of a nodehandle. This woll take care of
//all the node initializations.
float linear_vel,ang_vel;
long left_rpm,right_rpm;
void twist(const geometry_msgs::Twist &twist);
ros::NodeHandle n;
//since the odom data has to published we use a tf object to send 
//transform of base_link->mobile_base to the ROS core
//since we are going to publish both left and right and right encoder ticks
//using two data types of float32
robot::ticks data;
//creating publisher initializers on topic 
ros::Publisher ard_data1("ticks", &data);
ros::Subscriber<geometry_msgs::Twist> twist1("cmd_vel",twist);
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
}


void loop() {

digitalWrite(7, LOW);
digitalWrite(6, HIGH);
digitalWrite(10, LOW);
digitalWrite(11, HIGH);
analogWrite(8,left_rpm);
analogWrite(9,right_rpm);
data.right_wheel=rightWheel.read();
data.left_wheel=leftWheel.read();
n.spinOnce();
ard_data1.publish(&data);

}
void twist(const geometry_msgs::Twist &twist)
{
  linear_vel=twist.linear.x;
  ang_vel=twist.angular.z;
  double linear_vel_mins = linear_vel * 60;
  double angular_vel_mins = ang_vel * 60;
  double circumference = PI * 0.11;
  double tangential_vel = angular_vel_mins * 0.30;
  left_rpm=(linear_vel_mins / circumference) - (tangential_vel / circumference);
  right_rpm=(linear_vel_mins / circumference) + (tangential_vel / circumference);

}
  

