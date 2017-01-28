#include <Encoder.h>
#include <ros.h>
#include <robot/ticks.h>

//initializing with the help of a nodehandle. This woll take care of
//all the node initializations.

ros::NodeHandle n;
//since the odom data has to published we use a tf object to send 
//transform of base_link->mobile_base to the ROS core
//since we are going to publish both left and right and right encoder ticks
//using two data types of float32
robot::ticks data;
//creating publisher initializers on topic 
ros::Publisher ard_data1("ticks", &data);

Encoder leftWheel(2, 3);
Encoder rightWheel(18, 19);
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
}


void loop() {
analogWrite(8,255);
analogWrite(9,255);
digitalWrite(7, HIGH);
digitalWrite(6, LOW);
digitalWrite(10, HIGH);
digitalWrite(11, LOW);
data.right_wheel=rightWheel.read();

data.left_wheel=leftWheel.read();
n.spinOnce();
ard_data1.publish(&data);

}
