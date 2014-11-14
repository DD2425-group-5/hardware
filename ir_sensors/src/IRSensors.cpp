#include "IRSensors.hpp"

/*void mazerunner:feedbackCallback(int argc, char *argv[]){

}*/

void IRSensors::sensorCallback(const ras_arduino_msgs::ADConverter msg){
	int tmp[] ={msg.ch1, msg.ch2, msg.ch3, msg.ch4, msg.ch7, msg.ch8};
	for(int i=0; i<6; i++){
		//sensors[i].calculateDistance(tmp[i]);
		sensors[i].calculateDistanceExp(tmp[i]);
	}
	ROS_INFO("sensor distance: 1: [%f] 2: [%f] 3: [%f] 4: [%f] 5: [%f] 6: [%f] \n\n",\
	sensors[0].get_distance(),\
	sensors[1].get_distance(),\
	sensors[2].get_distance(),\
	sensors[3].get_distance(),\
	sensors[4].get_distance(),\
	sensors[5].get_distance());
}

void IRSensors::runNode(){

	ros::Rate loop_rate(10);	//10 Hz
	while (ros::ok())			//main loop of this code
	{
		

	

		ros::spinOnce();
		loop_rate.sleep();
	}
}

IRSensors::IRSensors(int argc, char *argv[]){
	ros::init(argc, argv, "ir_sensors");	// Name of node
	ros::NodeHandle handle;			// Handle node

	ROSUtil::getParam(handle, "/sensorcalib/a0", a0);
	ROSUtil::getParam(handle, "/sensorcalib/b0", b0);
    	ROSUtil::getParam(handle, "/sensorcalib/c0", c0);
	ROSUtil::getParam(handle, "/sensorcalib/d0", d0);
    	ROSUtil::getParam(handle, "/sensorcalib/a1", a1);
    	ROSUtil::getParam(handle, "/sensorcalib/b1", b1);
    	ROSUtil::getParam(handle, "/sensorcalib/c1", c1);
    	ROSUtil::getParam(handle, "/sensorcalib/d1", d1);
    	ROSUtil::getParam(handle, "/sensorcalib/a2", a2);
    	ROSUtil::getParam(handle, "/sensorcalib/b2", b2);
    	ROSUtil::getParam(handle, "/sensorcalib/c2", c2);
    	ROSUtil::getParam(handle, "/sensorcalib/d2", d2);
    	ROSUtil::getParam(handle, "/sensorcalib/a3", a3);
    	ROSUtil::getParam(handle, "/sensorcalib/b3", b3);
    	ROSUtil::getParam(handle, "/sensorcalib/c3", c3);
    	ROSUtil::getParam(handle, "/sensorcalib/d3", d3);
    	ROSUtil::getParam(handle, "/sensorcalib/a4", a4);
    	ROSUtil::getParam(handle, "/sensorcalib/b4", b4);
    	ROSUtil::getParam(handle, "/sensorcalib/c4", c4);
    	ROSUtil::getParam(handle, "/sensorcalib/d4", d4);
    	ROSUtil::getParam(handle, "/sensorcalib/a5", a5);
    	ROSUtil::getParam(handle, "/sensorcalib/b5", b5);
    	ROSUtil::getParam(handle, "/sensorcalib/c5", c5);
    	ROSUtil::getParam(handle, "/sensorcalib/d5", d5);


	/* Setup sensor calibration, using exponential fitting */

	sensors[0].calibrateExp(a0, b0, c0, d0, false);	
	sensors[1].calibrateExp(a1, b1, c1, d1, false);	
	sensors[2].calibrateExp(a2, b2, c2, d2, false);	
	sensors[3].calibrateExp(a3, b3, c3, d3, false);

	sensors[4].calibrateExp(a4, b4, c4, d4, true);
	sensors[5].calibrateExp(a5, b5, c5, d5, true);
   	
	runNode();
}

int main(int argc, char *argv[]) 
{
    IRSensors IRSensors(argc, argv);
}
