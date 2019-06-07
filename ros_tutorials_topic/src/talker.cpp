#include "ros/ros.h"
#include "std_msgs/Float64.h"
 

 
int main(int argc, char **argv)
{
  ros::init(argc, argv, "talker");
 
  ros::NodeHandle n;
 
  ros::Publisher pub = n.advertise<std_msgs::Float64>("/open_manipulator/joint3_position/command", 1000);
 
  ros::Rate loop_rate(10);
  
  float count = 0;
  while (ros::ok())
  {
    std_msgs::Float64 msg;
 
    msg.data = count;
 
    ROS_INFO("%f", msg.data);
 
    pub.publish(msg);
 
    ros::spinOnce();
 
    loop_rate.sleep();
    count = count+0.05;
  }
 
 
  return 0;
}
