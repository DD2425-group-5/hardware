#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include "geometry_msgs/Twist.h"
#include "ras_arduino_msgs/ADConverter.h"
#include "ir_sensors/IRDists.h"
#include "sensor.cpp"
#include <rosutil/rosutil.hpp>
#include <math.h>
#include <sstream>

class IRSensors {
public:
	IRSensors(int argc, char *argv[]);
	
private:
	std::vector<sensor> sensors;
	ros::Subscriber sub_adc;	//sub to get distance values
	ros::Publisher pub_dists;
	
	void runNode();
	void sensorCallback(const ras_arduino_msgs::ADConverter msg);
};
