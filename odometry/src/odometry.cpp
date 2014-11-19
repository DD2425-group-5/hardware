#include "odometry.hpp"

void Odometry::sensorCallback(const hardware_msgs::IRDists msg){
	
}

void Odometry::isTurningCallback(const std_msgs::Bool msg){
    ROS_INFO("GOT MESSAGE %d",msg.data);
}

void Odometry::encoderCallback(const ras_arduino_msgs::Encoders enc){
	ROS_INFO("ENCODER");
	if(startL==-1){
		startL=enc.encoder1;
	}
	if(startR==-1){
		startR=enc.encoder2;
	}
	int tmpL=0;
	int tmpR=0;
	tmpL=startL-enc.encoder1;
	tmpR=startR-enc.encoder2;
	ROS_INFO("L dist: %d R dist: %d",tmpL,tmpR);
}

void Odometry::runNode(){
    ros::Rate loop_rate(50);	//10 Hz
    while (ros::ok())			//main loop of this code
	{
		
		//geometry_msgs::Twist msg;	//for controlling the motor
		
		//msg.linear.x = 0.0;
		//msg.angular.y = 179;
		//msg.angular.z = 0.0;
		
		//pub_motor.publish(msg);		//pub to motor

		ros::spinOnce();
		loop_rate.sleep();
	}
    while(1){}
}

Odometry::Odometry(int argc, char *argv[]){
    ros::init(argc, argv, "odometry");	//name of node
    ros::NodeHandle handle;					//the handle
	
	startL=-1;
	startR=-1;
	
    //pub_motor = handle.advertise<geometry_msgs::Twist>("/motor3/twist", 1000);
    sub_sensor = handle.subscribe("/ir_sensors/IRDists", 1000, &Odometry::sensorCallback, this);
    sub_isTurning = handle.subscribe("/motor3/is_turning", 1, &Odometry::isTurningCallback, this);
    sub_encoder = handle.subscribe("/arduino/encoders", 1, &Odometry::encoderCallback, this);
    pub_odom = handle.advertise<hardware_msgs::Odometry>("/odometry/odometry", 1);
	
    runNode();
}

int main(int argc, char *argv[]) 
{
    Odometry odom(argc, argv);
}
