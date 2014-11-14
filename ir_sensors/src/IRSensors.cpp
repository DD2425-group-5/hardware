#include "IRSensors.hpp"

void IRSensors::sensorCallback(const ras_arduino_msgs::ADConverter msg){
	
	int tmp[] ={msg.ch1, msg.ch2, msg.ch3, msg.ch4, msg.ch7, msg.ch8};
	for(int i = 0; i<6; i++){
		sensors[i].calculateDistanceExp(tmp[i]);
	}
	
	ir_sensors::IRDists output;
	
	output.s0 = sensors[0].get_distance();
	output.s1 = sensors[1].get_distance();
	output.s2 = sensors[2].get_distance();
	output.s3 = sensors[3].get_distance();
	output.s4 = sensors[4].get_distance();
	output.s5 = sensors[5].get_distance();

	pub_dists.publish(output);

}

void IRSensors::runNode(){

	ros::Rate loop_rate(10);	//10 Hz
	while (ros::ok())		//main loop of this code
	{
		

	

		ros::spinOnce();
		loop_rate.sleep();
	}
}

IRSensors::IRSensors(int argc, char *argv[]){
	ros::init(argc, argv, "ir_sensors");	// Name of node
	ros::NodeHandle handle;			// Handle node
	
	sensors = std::vector<sensor>(6);

	double a0, b0, c0, d0;
	double a1, b1, c1, d1;
	double a2, b2, c2, d2;
	double a3, b3, c3, d3;
	double a4, b4, c4, d4;
	double a5, b5, c5, d5;

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

	sensors[0] = sensor(a0, b0, c0, d0, false);	
	sensors[1] = sensor(a1, b1, c1, d1, false);	
	sensors[2] = sensor(a2, b2, c2, d2, false);	
	sensors[3] = sensor(a3, b3, c3, d3, false);

	sensors[4] = sensor(a4, b4, c4, d4, true);
	sensors[5] = sensor(a5, b5, c5, d5, true);
   	
	sub_adc = handle.subscribe("/arduino/adc", 1000, &IRSensors::sensorCallback, this);
	pub_dists = handle.advertise<ir_sensors::IRDists>("/ir_sensors/dists", 1000);
	runNode();
}

int main(int argc, char *argv[]) {

    IRSensors IRSensors(argc, argv);

}
