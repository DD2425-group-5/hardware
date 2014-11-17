#include "odometry.hpp"

void Odometry::sensorCallback(const ir_sensors::IRDists msg){
	
}

void Odometry::isTurningCallback(const std_msgs::Bool msg){
	ROS_INFO("GOT MESSAGE %d",msg.data);
}

void Odometry::encoderCallback(const ras_arduino_msgs::Encoders enc){
	
}

void Odometry::runNode(){
	ros::Rate loop_rate(10);	//10 Hz
	/*while (ros::ok())			//main loop of this code
	{
		geometry_msgs::Twist msg;	//for controlling the motor
		
		msg.linear.x = 0.0;
		msg.angular.y = 179;
		msg.angular.z = 0.0;
		
		pub_motor.publish(msg);		//pub to motor

		ros::spinOnce();
		loop_rate.sleep();
	}*/
	while(1){}
}

Odometry::Odometry(int argc, char *argv[]){
	ros::init(argc, argv, "odometry");	//name of node
	ros::NodeHandle handle;					//the handle
	
	//pub_motor = handle.advertise<geometry_msgs::Twist>("/motor3/twist", 1000);
	sub_sensor = handle.subscribe("/ir_sensors/IRDists", 1000, &Odometry::sensorCallback, this);
	sub_isTurning = handle.subscribe("/motor3/is_turning", 1, &Odometry::isTurningCallback, this);
	sub_encoder = handle.subscribe("/arduino/encoders", 1, &Odometry::encoderCallback, this);
	
	runNode();
}

int main(int argc, char *argv[]) 
{
    Odometry odom(argc, argv);
}
