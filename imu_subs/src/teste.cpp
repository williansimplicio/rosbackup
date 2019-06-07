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
  
  AcX = msg -> linear_acceleration.x;
  AcY = msg -> linear_acceleration.y;
  AcZ = msg -> linear_acceleration.z;

  Roll = atan2(AcY, AcZ);
  Pitch = atan2(-AcX, sqrt(AcY*AcY+AcZ*AcZ));
  
	//garra = ((Pitch+1.4)/2.8)*0.03 - 0.01;
		
	/*Roll = msg->orientation.x;
	Pitch = msg->orientation.y;
	Yaw = msg->orientation.z;*/
  
}

int main(int argc, char **argv) {
  
  ros::init(argc, argv, "teste");
  
  ros::NodeHandle n;
  ros::Rate loop_rate(100);
	Listener Celular1, Celular2;

  ros::Subscriber sub = n.subscribe("imu/data", 1000, &Listener::callback, &Celular1);
  
	//ros::Subscriber sub = n.subscribe("/imu/data", 1000, &Listener::callback, &Celular1);

	ros::Subscriber sub2 = n.subscribe("edTopic", 1000, &Listener::callback, &Celular2);

	//Juntas
	ros::Publisher pub4 = n.advertise<std_msgs::Float64>("/open_manipulator/joint4_position/command", 1000);
	
	ros::Publisher pubRoll = n.advertise<std_msgs::Float64>("roll", 1000);
	ros::Publisher pubPitch = n.advertise<std_msgs::Float64>("pitch", 1000);

	//Juntas
  std_msgs::Float64 Junta4;

	std_msgs::Float64 roll;
	std_msgs::Float64 pitch;

	
	while (ros::ok()) {
		
		ros::spinOnce();

		//ROS_INFO("Angulo Celular1 [%.2f], Angulo Celular2 [%.2f]\n", Celular1.Pitch*180.0/M_PI, Celular2.Pitch*180.0/M_PI);

		//ROS_INFO("Celular2-Celular1: [%.2f]graus\n", Celular2.Roll - Celular1.Roll);

		//Junta4.data = Celular2.Pitch - Celular1.Pitch;

		//pub4.publish(Junta4);

		ROS_INFO("\nROLL: [%.2f]\nPITCH: [%.2f]\n", Celular1.Roll*180/M_PI, Celular1.Pitch*180/M_PI);
		
		roll.data = Celular1.Roll*180/M_PI;
		pitch.data = Celular1.Pitch*180/M_PI;
			
		pubRoll.publish(roll);
		pubPitch.publish(pitch);
		loop_rate.sleep();		
	}
  
  return 0;
}
