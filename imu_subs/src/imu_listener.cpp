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

	//garra = ((Pitch+1.4)/2.8)*0.03 - 0.01;

  //ROS_INFO("Rotacoes Roll: [%f], Pitch: [%f], Yaw: [%f]\n", Roll, Pitch, Rz);	

	//obtenção das orientações	
	/*Roll = msg->orientation.x;
	Pitch = msg->orientation.y;
	Yaw = msg->orientation.z;*/
}

int main(int argc, char **argv) {
  
	//referencia eixo Z
	double YawReference;	
	
	//juntas  
	std_msgs::Float64 Junta1;
	std_msgs::Float64 Junta3;
	std_msgs::Float64 Junta4;

	//garras
	std_msgs::Float64 Garra1;
	std_msgs::Float64 Garra2;	
	  

	ros::init(argc, argv, "imu_listener");//inicia o nó
  
  ros::NodeHandle n;//cria objeto n do tipo nodehandle para gerenciar subscriber e publisher
  
	ros::Rate loop_rate(10);//define frequencia de 10 Hz
	
	Listener listener;//cria objeto do tipo listener;

  ros::Subscriber sub = n.subscribe("/android/imu", 1000, &Listener::callback, &listener);//Subscriber dos valores de orientação 

  ros::Publisher pub3 = n.advertise<std_msgs::Float64>("/open_manipulator/joint3_position/command", 1000);//Publisher da Junta3

	ros::Publisher pub1 = n.advertise<std_msgs::Float64>("/open_manipulator/joint1_position/command", 1000);//Publisher da Junta1

	ros::Publisher pub4 = n.advertise<std_msgs::Float64>("/open_manipulator/joint4_position/command", 1000);//Publisher da Junta4

	//Publisher das Garras
	ros::Publisher Garra1_pub = n.advertise<std_msgs::Float64>("/open_manipulator/gripper_position/command", 1000);
	ros::Publisher Garra2_pub = n.advertise<std_msgs::Float64>("/open_manipulator/gripper_sub_position/command", 1000);

	
  //ros::Rate loop_rate(10);

	for (int i=0; i<5; i++) {
		ros::spinOnce();		
		YawReference = listener.Yaw + YawReference;
		ROS_INFO("Reference :%.4f\n", YawReference);		
  	loop_rate.sleep();
	}
		
		YawReference = YawReference/5;  

  	while (ros::ok()) { 	
		
		ros::spinOnce();

		//valores de orientação são associados às variaveis das juntas
		Junta1.data = listener.Yaw;		
		Junta4.data = listener.Pitch;

		if (listener.Roll < 0.7853 && listener.Roll > -0.94)
			Junta3.data = listener.Roll;
		else if ( listener.Roll < -0.94 )		
			Junta3.data =	-0.94;	
		else if ( listener.Roll > 0.7853 )		
			Junta3.data =	0.7853;	
				
		//publicação dos valores em cada junta
		
		//pub1.publish(Junta1);		
		pub3.publish(Junta3);
		//pub4.publish(Junta4);

		//ROS_INFO("JUNTA3 :%.4f\n", Junta3.data);
 		ROS_INFO("JUNTA3 :%.4f\n", Junta3.data * 180.0/M_PI); 


		//publicação valor das garras
		/*	
		ROS_INFO("Garra :%.4f\n", listener.garra);		
			
		Garra1.data = listener.garra;
		Garra2.data = listener.garra;
		
		Garra1_pub.publish(Garra1);
		Garra2_pub.publish(Garra2);*/
		
		//loop_rate.sleep();

}
	
	ros::spin();
  
  return 0;
}
