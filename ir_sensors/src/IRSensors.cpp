#include "IRSensors.hpp"

void IRSensors::sensorCallback(const ras_arduino_msgs::ADConverter msg){
    // Construct IR distances message and store it in the 
    output = hardware_msgs::IRDists();

    output.s0 = sensors[0].calculateDistanceExp(msg.ch1);
    output.s1 = sensors[1].calculateDistanceExp(msg.ch2);
    output.s2 = sensors[2].calculateDistanceExp(msg.ch3);
    output.s3 = sensors[3].calculateDistanceExp(msg.ch4);
    output.s4 = sensors[4].calculateDistanceExp(msg.ch7);
    output.s5 = sensors[5].calculateDistanceExp(msg.ch8);
}

void IRSensors::runNode(){

    ros::Rate loop_rate(50);	//50 Hz
    while (ros::ok())		//main loop of this code
    {
        ros::spinOnce();
        // publish the latest data from the IR sensors
        pub_dists.publish(output);

        loop_rate.sleep();
    }
}

IRSensors::IRSensors(int argc, char *argv[]){
    ros::init(argc, argv, "ir_sensors");	// Name of node
    ros::NodeHandle handle;			// Handle node
	
    sensors = std::vector<Sensor>(6);
    output = hardware_msgs::IRDists();

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

    // Create sensors with the coefficients from the parameter file
    sensors[0] = Sensor(a0, b0, c0, d0, false);	
    sensors[1] = Sensor(a1, b1, c1, d1, false);	
    sensors[2] = Sensor(a2, b2, c2, d2, false);	
    sensors[3] = Sensor(a3, b3, c3, d3, false);
    sensors[4] = Sensor(a4, b4, c4, d4, true);
    sensors[5] = Sensor(a5, b5, c5, d5, true);
   	
    // The adc subscriber has a queue length of 1 - it should only process the
    // latest message when ros spins.
    sub_adc = handle.subscribe("/arduino/adc", 1, &IRSensors::sensorCallback, this);
    pub_dists = handle.advertise<hardware_msgs::IRDists>("/ir_sensors/dists", 1000);
    runNode();
}

int main(int argc, char *argv[]) {
    IRSensors IRSensors(argc, argv);
}
