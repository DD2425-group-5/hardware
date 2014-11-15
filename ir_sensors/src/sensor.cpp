#include "sensor.hpp"

// For exponential calibration curve:
Sensor::Sensor(double a, double b, double c, double d, bool lrange) : 
    a(a), b(b), c(c), d(d), lrange(lrange){
}

float Sensor::calculateDistanceExp(int val){
    value = val;
	
    distance = a*std::exp(b*val) + c*std::exp(d*val);
    if (lrange){
        if (distance > 80){
            distance = 150;
        } else if (distance < 10){
            distance = -1;  
        }
    } else {
        if (distance > 30){
            distance = 150;
        } else if (distance < 4){
            distance = -1;
        }
    }
    return distance;
}

int Sensor::get_value(){
    return value;
}

double Sensor::get_distance(){
    return distance;
}
