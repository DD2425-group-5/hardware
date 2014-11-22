#include "odometry.hpp"

/*void Odometry::sensorCallback(const hardware_msgs::IRDists msg){
	
}*/

void Odometry::isTurningCallback(const controller_msgs::Turning msg){
    //ROS_INFO("GOT MESSAGE %f",msg.degrees);
	if(msg.isTurning){
		isTurning=1;
	}
	else{
		isTurning=0;
	}
}

void Odometry::encoderCallback(const ras_arduino_msgs::Encoders enc){
	//ROS_INFO("ENCODER %f",wheelRadius);
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
	float avgTicks = ((float)tmpL+(float)tmpR)/2.0;
	distanceSinceLast = distance;
	distance = (avgTicks/180)*(wheelRadius*3.1415);
	distanceSinceLast = distance-distanceSinceLast;
	//ROS_INFO("L dist: %d R dist: %d dist: %f cm",tmpL,tmpR,distance);
}

void Odometry::runNode(){
    ros::Rate loop_rate(50);	//10 Hz
    while (ros::ok())			//main loop of this code
	{
		
		hardware_msgs::Odometry msg;	//for controlling the motor
		
		msg.distanceTotal = distance;
		msg.distanceFromLast = distanceSinceLast;
		
		if(!isTurning){
			pub_odom.publish(msg);		//pub to odometry
		}
		
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
	distance=0.0;
	isTurning=0;
	
	std::string encoder_sub_topic;
    ROSUtil::getParam(handle, "/topic_list/robot_topics/published/encoder_topic", encoder_sub_topic);
	std::string odometry_pub_topic;
    ROSUtil::getParam(handle, "/topic_list/hardware_topics/odometry/published/odometry_topic", odometry_pub_topic);
	std::string turn_sub_topic;
    ROSUtil::getParam(handle, "/topic_list/controller_topics/wallfollower/published/turning_topic", turn_sub_topic);
	ROSUtil::getParam(handle, "/robot_info/wheel_radius", wheelRadius);
	
    //pub_motor = handle.advertise<geometry_msgs::Twist>("/motor3/twist", 1000);
    //sub_sensor = handle.subscribe("/ir_sensors/IRDists", 1000, &Odometry::sensorCallback, this);
    sub_isTurning = handle.subscribe(turn_sub_topic, 1, &Odometry::isTurningCallback, this);
    sub_encoder = handle.subscribe(encoder_sub_topic, 1, &Odometry::encoderCallback, this);
    pub_odom = handle.advertise<hardware_msgs::Odometry>(odometry_pub_topic, 1);
	
    runNode();
}

int main(int argc, char *argv[]) 
{
    Odometry odom(argc, argv);
}
