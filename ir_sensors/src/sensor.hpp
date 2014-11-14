#ifndef sensor_hpp
#define sensor_hpp
#include <cmath>

class sensor {
public:
	sensor(){}
	double get_distance();
	int get_value();
	int get_number();
	
	sensor(double a, double b, double c, double d, bool lrange);
	void calculateDistanceExp(int val);
private:
	double distance;
	int value;
	double a, b, c, d;
	bool lrange;
};
#endif //sensor_hpp
