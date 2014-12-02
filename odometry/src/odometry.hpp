#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/PoseStamped.h"
#include "ras_arduino_msgs/ADConverter.h"
#include "ras_arduino_msgs/Encoders.h"
#include "std_msgs/Bool.h"
#include "hardware_msgs/IRDists.h"
#include "hardware_msgs/Odometry.h"
#include "controller_msgs/Turning.h"
#include <rosutil/rosutil.hpp>
#include <sstream>
#include <cmath>
#include "mathutil/mathutil.hpp"
#include <Eigen/Geometry>


class Odometry {
public:
    Odometry(int argc, char *argv[]);
	
private:
	int startL;		//left encoder at beginning of segment
	int startR;		//right -||-
	int lastL;		//last L encoder value
	int lastR;		//-||- R -||-
	int originalL;
	int originalR;
	float xNew;
    float yNew;
    float thetaNew;
    float thetaOld;
    float xOld;
    float yOld;
	
	float wheelRadius;
	float linearDistanceL;
	float linearDistanceR;
	float robotBase;
	float distance;
	float distanceSinceLast;
	int isTurning;
	
    ros::Subscriber sub_sensor;	//sub to get distance values
    ros::Subscriber sub_encoder;// for encoder feedback
    ros::Subscriber sub_isTurning;// for encoder feedback
    ros::Publisher pub_odom;
    ros::Publisher pub_pose;
	
    void runNode();
    //void sensorCallback(const hardware_msgs::IRDists msg);
    void encoderCallback(const ras_arduino_msgs::Encoders feedback);
    void isTurningCallback(const controller_msgs::Turning msg);
};
