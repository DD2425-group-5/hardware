#include "odometry.hpp"

/*void Odometry::sensorCallback(const hardware_msgs::IRDists msg){
	
}*/

void Odometry::isTurningCallback(const controller_msgs::Turning msg){
    //ROS_INFO("GOT MESSAGE %f",msg.degrees);
	if(msg.isTurning){
		isTurning=1;
		startL=-1;
		startR=-1;
		distance=0.0;
		distanceSinceLast=0.0;
	}
	else{
		isTurning=0;
	}
}

void Odometry::encoderCallback(const ras_arduino_msgs::Encoders enc){
	//ROS_INFO("ENCODER %f",wheelRadius);
	if(!isTurning){
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
		distance = (avgTicks/180)*(wheelRadius*M_PI);
		distanceSinceLast = distance-distanceSinceLast;
		//ROS_INFO("L dist: %d R dist: %d dist: %f cm",tmpL,tmpR,distance);
	}
	if(lastL==-1){
		lastL = enc.encoder1;
	}
	if(lastR==-1){
		lastR = enc.encoder2;
	}
	if(originalL==-1){
		originalL = enc.encoder1;
		xNew = 0.0;
        yNew= 0.0;
        thetaNew = 0.0;
        thetaOld = 0.0;
        xOld = 0.0;
        yOld= 0.0;
        linearDistanceL = 0;
        linearDistanceR = 0;
        ROS_INFO("Stuff is reset to 0!");
	}
	if(originalR==-1){
		originalR = enc.encoder2;
	}
	if (lastL!=-1 || lastR!=-1){
		int changeL = lastL - enc.encoder1;
		int changeR = lastR - enc.encoder2;
		int tmp1 = originalL - enc.encoder1;
		int tmp2 = originalR - enc.encoder2;
		
		ROS_INFO("changeL = %d changeR = %d diff = %d",changeL,changeR,changeL-changeR);
		
		//calculate the angle Theta the robot has steered for the current time interval
		linearDistanceL = float(changeL)*M_PI*wheelRadius/180.0;
		linearDistanceR = float(changeR)*M_PI*wheelRadius/180.0;
		ROS_INFO("LinDistanceL=%f, LinDistanceR=%f, difference R-L = %f",linearDistanceL,linearDistanceR, linearDistanceR-linearDistanceL );
		
        if(MathUtil::approxEqual(linearDistanceR,linearDistanceL, 0.00000001)){
            xNew = xOld + cos(thetaOld)*(linearDistanceR+linearDistanceL)/2;
            yNew = yOld + sin(thetaOld)*(linearDistanceR+linearDistanceL)/2;
            thetaNew = thetaOld;
        }
        else{  
            ROS_INFO("NORMAL WORKING CONDITION");
            xNew = xOld+\
            robotBase*((linearDistanceR+linearDistanceL)/(2.0*(linearDistanceR-linearDistanceL)))*\
            ( sin( (linearDistanceR-linearDistanceL)/robotBase + thetaOld) -\
              sin( thetaOld) );
             
            yNew = yOld-\
            robotBase*((linearDistanceR+linearDistanceL)/(2.0*(linearDistanceR-linearDistanceL)))*\
            ( cos( (linearDistanceR-linearDistanceL)/robotBase + thetaOld) -\
              cos( thetaOld) );
            
            thetaNew = thetaOld+((linearDistanceR-linearDistanceL)/robotBase);
        }
		//ROS_INFO("L = %d R = %d diff = %d",tmp1,tmp2,tmp1-tmp2);
		//publish
		ROS_INFO("X=%f, Y=%f, Theta=%f", xNew, yNew, (thetaNew*180.0/M_PI));
		xOld=xNew;
		yOld=yNew;
		thetaOld=thetaNew;
		
		lastL = enc.encoder1;
		lastR = enc.encoder2;
		//loop
	}
}

void Odometry::runNode(){
    ros::Rate loop_rate(50);	//10 Hz
    while (ros::ok())			//main loop of this code
	{
		
		hardware_msgs::Odometry msg;	//for controlling the motor
		
		msg.distanceTotal = distance;
		msg.distanceFromLast = distanceSinceLast;
		msg.totalX = xNew;
		msg.totalY = yNew;
		msg.latestHeading = thetaNew;
		
		Eigen::Quaternionf eigenPose;
	    eigenPose = Eigen::AngleAxis<float>(thetaNew, Eigen::Vector3f::UnitZ());
		geometry_msgs::PoseStamped rvizPose;
		rvizPose.pose.orientation.x = eigenPose.x();
		rvizPose.pose.orientation.y = eigenPose.y();
		rvizPose.pose.orientation.z = eigenPose.z();
		rvizPose.pose.orientation.w = eigenPose.w();
		rvizPose.pose.position.x = xNew;
		rvizPose.pose.position.y = yNew;
		rvizPose.header.frame_id = "kur";
		if(!isTurning){
			pub_odom.publish(msg);		//pub to odometry
			pub_pose.publish(rvizPose);     //publish to rviz for pose
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
	distanceSinceLast=0.0;
	isTurning=0;
	
	lastL = -1;
	lastR = -1;
	
	originalL = -1;
	originalR = -1;
	
	std::string encoder_sub_topic;
    ROSUtil::getParam(handle, "/topic_list/robot_topics/published/encoder_topic", encoder_sub_topic);
	std::string odometry_pub_topic;
    ROSUtil::getParam(handle, "/topic_list/hardware_topics/odometry/published/odometry_topic", odometry_pub_topic);
    std::string pose_pub_topic;
    ROSUtil::getParam(handle, "/topic_list/hardware_topics/odometry/published/pose_topic", pose_pub_topic);
	std::string turn_sub_topic;
    ROSUtil::getParam(handle, "/topic_list/controller_topics/wallfollower/published/turning_topic", turn_sub_topic);
	ROSUtil::getParam(handle, "/robot_info/wheel_radius", wheelRadius);
    ROSUtil::getParam(handle, "/robot_info/wheel_baseline", robotBase);
	
	
    //pub_motor = handle.advertise<geometry_msgs::Twist>("/motor3/twist", 1000);
    //sub_sensor = handle.subscribe("/ir_sensors/IRDists", 1000, &Odometry::sensorCallback, this);
    sub_isTurning = handle.subscribe(turn_sub_topic, 1, &Odometry::isTurningCallback, this);
    sub_encoder = handle.subscribe(encoder_sub_topic, 1, &Odometry::encoderCallback, this);
    pub_odom = handle.advertise<hardware_msgs::Odometry>(odometry_pub_topic, 1);
    pub_pose = handle.advertise<geometry_msgs::PoseStamped>(pose_pub_topic, 10);
	
    runNode();
}

int main(int argc, char *argv[]) 
{
    Odometry odom(argc, argv);
}
