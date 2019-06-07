#include "ros/ros.h"
#include "sensor_msgs/Imu.h"
#include "std_msgs/Float64.h"
#include "math.h"

class Listener 
{
public:
double AcX, AcY, AcZ, Roll, Pitch, Yaw, garra; 

void callback(const sensor_msgs::Imu::ConstPtr& msg);
};

void Listener::callback(const sensor_msgs::Imu::ConstPtr& msg) {

AcX = msg->linear_acceleration.x;
AcY = msg->linear_acceleration.y;
AcZ = msg->linear_acceleration.z;


Roll = atan2(AcY, AcZ);
Pitch = atan2(-AcX, sqrt(AcY*AcY+AcZ*AcZ));

int main(int argc, char **argv) {

//juntas  
std_msgs::Float64 Junta1;
std_msgs::Float64 Junta2;
std_msgs::Float64 Junta3;
std_msgs::Float64 Junta4;

//garras
std_msgs::Float64 Garra1;
std_msgs::Float64 Garra2;	


ros::init(argc, argv, "imu_listener");//inicia o nó

ros::NodeHandle n;//cria objeto n do tipo nodehandle para gerenciar subscriber e publisher

ros::Rate loop_rate(10);//define frequencia de 10 Hz

Listener imu1, imu2, imu3;//cria objeto do tipo listener;

ros::Subscriber sub1 = n.subscribe("/android/imu", 1000, &Listener::callback, &imu1);
ros::Subscriber sub2 = n.subscribe("/android/imu", 1000, &Listener::callback, &imu2);
ros::Subscriber sub3 = n.subscribe("/android/imu", 1000, &Listener::callback, &imu3);
	

ros::Publisher pub1 = n.advertise<std_msgs::Float64>("/open_manipulator/joint1_position/command", 1000);
ros::Publisher pub2 = n.advertise<std_msgs::Float64>("/open_manipulator/joint2_position/command", 1000);
ros::Publisher pub3 = n.advertise<std_msgs::Float64>("/open_manipulator/joint3_position/command", 1000);
ros::Publisher pub4 = n.advertise<std_msgs::Float64>("/open_manipulator/joint4_position/command", 1000);

//Publisher das Garras
ros::Publisher Garra1_pub = n.advertise<std_msgs::Float64>("/open_manipulator/gripper_position/command", 1000);
ros::Publisher Garra2_pub = n.advertise<std_msgs::Float64>("/open_manipulator/gripper_sub_position/command", 1000);

while (ros::ok()) { 	

	ros::spinOnce();

	//valores de orientação são associados às variaveis das juntas
	Junta1.data = listener.Yaw;		
	Junta4.data = listener.Pitch;

	/*if (listener.Roll < 0.7853 && listener.Roll > -0.94)
	Junta3.data = listener.Roll;
	else if ( listener.Roll < -0.94 )		
	Junta3.data =	-0.94;	
	else if ( listener.Roll > 0.7853 )		
	Junta3.data =	0.7853;*/	

	pub1.publish(Junta1);		
	pub2.publish(Junta2);
	pub3.publish(Junta3);
	pub4.publish(Junta4);
	
	loop_rate.sleep();

}
	
return 0;

}
