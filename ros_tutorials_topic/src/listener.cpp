#include "ros/ros.h"
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/Float64.h"
 

class Listener {
  public:
    double joint1, joint2, joint3, joint4, gripper; 

    void chatterCallback(const std_msgs::Float32MultiArray::ConstPtr& msg);
};

void Listener::chatterCallback(const std_msgs::Float32MultiArray::ConstPtr& msg) {
  
	joint1 = msg->data[0];
	joint2 = msg->data[1];
	joint3 = msg->data[2];
	joint4 = msg->data[3];
	gripper = msg->data[4];
		
}
 
int main(int argc, char **argv)
{

	//juntas  
	std_msgs::Float64 Junta1;
	std_msgs::Float64 Junta2;
	std_msgs::Float64 Junta3;
	std_msgs::Float64 Junta4;

	//garras
	std_msgs::Float64 Garra1;
	std_msgs::Float64 Garra2;	


  ros::init(argc, argv, "listener");

	ros::NodeHandle n;
 
	ros::Rate loop_rate(1000);//define frequencia de 10 Hz
	
	Listener listener;//cria objeto do tipo listener;

  ros::Subscriber sub = n.subscribe("chatter", 1000, &Listener::chatterCallback, &listener);
 
	//publisher das juntas

	ros::Publisher pub1 = n.advertise<std_msgs::Float64>("/open_manipulator/joint1_position/command", 1000);//Publisher da Junta1

	ros::Publisher pub2 = n.advertise<std_msgs::Float64>("/open_manipulator/joint2_position/command", 1000);//Publisher da Junta2

	ros::Publisher pub3 = n.advertise<std_msgs::Float64>("/open_manipulator/joint3_position/command", 1000);//Publisher da Junta3

	ros::Publisher pub4 = n.advertise<std_msgs::Float64>("/open_manipulator/joint4_position/command", 1000);//Publisher da Junta3

	//Publisher das Garras
	
	ros::Publisher Garra1_pub = n.advertise<std_msgs::Float64>("/open_manipulator/gripper_position/command", 1000);
	ros::Publisher Garra2_pub = n.advertise<std_msgs::Float64>("/open_manipulator/gripper_sub_position/command", 1000);

	while (ros::ok()) {
  
		ros::spinOnce();
			
		Junta1.data = listener.joint1;		
		Junta2.data = listener.joint2;
		Junta3.data = listener.joint3;
		Junta4.data = listener.joint4;
		
		Garra1.data = listener.gripper;
		Garra2.data = listener.gripper;

		pub1.publish(Junta1);
		pub2.publish(Junta2);
		pub3.publish(Junta3);
		pub4.publish(Junta4);
		
		Garra1_pub.publish(Garra1);
		Garra2_pub.publish(Garra2);

		loop_rate.sleep();
	}	

 
  return 0;
}
