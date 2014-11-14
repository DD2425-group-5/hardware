#ifndef sensor_hpp
#define sensor_hpp
#include <cmath>

class sensor {
public:
	sensor(){}
	double get_distance();
	int get_value();
	int get_number();
	
	sensor(int num2, double a, double b, double c, double d, bool lrange);
	void calculateDistanceExp(int val2);
	void calibrateExp(double aa, double bb, double cc, double dd, bool llrange);
private:
	double distance;
	int value;
	int number;
	double a, b, c, d;
	bool lrange;
};
#endif //sensor_hpp
