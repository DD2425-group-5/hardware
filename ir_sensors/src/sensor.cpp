#include "sensor.hpp"

// For exponential calibration curve:
sensor::sensor(int num2, double a, double b, double c, double d, bool lrange) : 
	a(a), b(b), c(c), d(d), lrange(lrange){
	number = num2;
}

void sensor::calculateDistanceExp(int val2){
	value = val2;
	double tmp2 = 0;

	tmp2 = a*std::exp(b*val2) + c*std::exp(d*val2);

	distance = tmp2;
	if (distance > 30 && !lrange){
		distance = 30;
	}

}

void sensor::calibrateExp(double aa, double bb, double cc, double dd, bool llrange){

	a = aa;
	b = bb;
	c = cc;
	d = dd;
	lrange = llrange;

}

int sensor::get_number(){
	return number;
}

int sensor::get_value(){
	return value;
}

double sensor::get_distance(){
	return distance;
}
