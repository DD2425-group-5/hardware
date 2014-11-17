#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "ras_arduino_msgs/ADConverter.h"
#include "ras_arduino_msgs/Encoders.h"
#include "std_msgs/Bool.h"
#include "hardware_msgs/IRDists.h"
#include "hardware_msgs/Odometry.h"


class Odometry {
public:
    Odometry(int argc, char *argv[]);
	
private:
    ros::Subscriber sub_sensor;	//sub to get distance values
    ros::Subscriber sub_encoder;// for encoder feedback
    ros::Subscriber sub_isTurning;// for encoder feedback
    ros::Publisher pub_odom;
	
    void runNode();
    void sensorCallback(const hardware_msgs::IRDists msg);
    void encoderCallback(const ras_arduino_msgs::Encoders feedback);
    void isTurningCallback(const std_msgs::Bool msg);
};
