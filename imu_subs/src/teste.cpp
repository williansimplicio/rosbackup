#include "ros/ros.h"
#include "sensor_msgs/Imu.h"
#include "std_msgs/Float64.h"
#include "math.h"

//Class to use values of callback function
class Listener {
public:
double AcX, AcY, AcZ, Roll, Pitch, Yaw; 
	
void callback(const sensor_msgs::Imu::ConstPtr& msg);
};

void Listener::callback(const sensor_msgs::Imu::ConstPtr& msg) {

AcX = msg -> linear_acceleration.x;
AcY = msg -> linear_acceleration.y;
AcZ = msg -> linear_acceleration.z;

Roll = atan2(AcY, AcZ);
Pitch = atan2(-AcX, sqrt(AcY*AcY+AcZ*AcZ));

}

int main(int argc, char **argv) {

ros::init(argc, argv, "teste");

ros::NodeHandle n;
//100Hz Frequency
ros::Rate loop_rate(100);

//Objects to use values from callback
Listener Imu1, Imu2, Imu3;

//IMU SUBSCRIBER
ros::Subscriber sub1 = n.subscribe("imu/data", 1000, &Listener::callback, &Imu1);
//ros::Subscriber sub2 = n.subscribe("imu/data", 1000, &Listener::callback, &Imu2);
//ros::Subscriber sub3 = n.subscribe("imu/data", 1000, &Listener::callback, &Imu2);	

//DebugPublisher
ros::Publisher pubRoll = n.advertise<std_msgs::Float64>("roll", 1000);
ros::Publisher pubPitch = n.advertise<std_msgs::Float64>("pitch", 1000);

//DebugVariables
std_msgs::Float64 roll;
std_msgs::Float64 pitch;


while (ros::ok()) {

	ros::spinOnce();

	ROS_INFO("\nROLL: [%.2f]\nPITCH: [%.2f]\n", Imu1.Roll*180/M_PI, Imu1.Pitch*180/M_PI);

	roll.data = Imu1.Roll*180/M_PI;
	pitch.data = Imu1.Pitch*180/M_PI;

	pubRoll.publish(roll);
	pubPitch.publish(pitch);
	loop_rate.sleep();		
}

return 0;
}
