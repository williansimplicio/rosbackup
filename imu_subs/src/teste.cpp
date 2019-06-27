#include "ros/ros.h"
#include "sensor_msgs/Imu.h"
#include "geometry_msgs/Vector3Stamped.h"
#include "std_msgs/Float64.h"
#include "math.h"

class Listener {
	public:
	double AcX, AcY, AcZ, Roll, Pitch, Yaw, garra; 

	void callback(const sensor_msgs::Imu::ConstPtr& msg);
	void YAWcallback(const geometry_msgs::Vector3Stamped::ConstPtr& msg);
};

void Listener::callback(const sensor_msgs::Imu::ConstPtr& msg) {

	AcX = msg -> linear_acceleration.x;
	AcY = msg -> linear_acceleration.y;
	AcZ = msg -> linear_acceleration.z;

	Roll = atan2(AcY, AcZ);
	Pitch = atan2(-AcX, sqrt(AcY*AcY+AcZ*AcZ));
	
	//garra = ((Pitch+1.4)/2.8)*0.03 - 0.01;
}

void Listener::YAWcallback(const geometry_msgs::Vector3Stamped::ConstPtr& msg){

	Yaw = msg -> vector.z;
}

int main(int argc, char **argv) {

ros::init(argc, argv, "teste");

ROS_INFO("Initialing node");
ros::NodeHandle n;
ros::Rate loop_rate(10);
Listener Celular1, Celular2, Celular3;

//ros::Subscriber sub1 = n.subscribe("Imu1", 1000, &Listener::callback, &Celular1); //Punho

//ros::Subscriber sub2 = n.subscribe("Imu2", 1000, &Listener::callback, &Celular2); //Antebraço

ros::Subscriber sub3 = n.subscribe("android/imu", 1000, &Listener::callback, &Celular3); //Ombro

//ros::Subscriber subYaw = n.subscribe("/imu/rpy/filtered", 1000, &Listener::YAWcallback, &Celular3); //Yaw

//Juntas
ros::Publisher pub4 = n.advertise<std_msgs::Float64>("/open_manipulator/joint4_position/command", 1000);
ros::Publisher pub3 = n.advertise<std_msgs::Float64>("/open_manipulator/joint3_position/command", 1000);
ros::Publisher pub2 = n.advertise<std_msgs::Float64>("/open_manipulator/joint2_position/command", 1000);
ros::Publisher pub1 = n.advertise<std_msgs::Float64>("/open_manipulator/joint1_position/command", 1000);


//Juntas
std_msgs::Float64 Junta4;
std_msgs::Float64 Junta3;
std_msgs::Float64 Junta2;
std_msgs::Float64 Junta1;


while (ros::ok()) {

	ros::spinOnce();

	/*Angulo Antebraço - Angulo do Punho
	ROS_INFO("Angulo Celular1 [%.2f], Angulo Celular2 [%.2f]\n", Celular1.Roll*180.0/M_PI, Celular2.Roll*180.0/M_PI;
	ROS_INFO("Celular2-Celular1: [%.2f]graus\n", (Celular2.Roll - Celular1.Roll)*180.0/M_PI);
	Junta4.data = -(Celular2.Roll - Celular1.Roll); //Publicar simétrico do resultado*/

	ROS_INFO("Roll [%.2f]\n", Celular3.Roll);

	/*Angulo Ombro - Angulo Antebraço
	ROS_INFO("Angulo Celular2 [%.2f], Angulo Celular3 [%.2f]\n", Celular2.Roll*180.0/M_PI, Celular3.Roll*180.0/M_PI);
	Junta3.data = -(Celular3.Roll - Celular2.Roll);
	ROS_INFO("Celular3-Celular2: [%.2f]graus\n", (Celular3.Roll - Celular2.Roll)*180.0/M_PI);*/

	/*Junta2.data = Celular3.Roll;
	Junta1 = Celular3.Yaw

	pub4.publish(Junta4);
	pub3.publish(Junta3);
	pub2.publish(Junta2)
	pub1.publish(Junta1)*/

	loop_rate.sleep();
}

return 0;
}
