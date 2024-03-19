#include "DistanceSensor.h"

using namespace Sensor;

DistanceSensor::DistanceSensor(uint32_t echoPin, uint32_t trigPin) :
    m_trigPin{ trigPin }, m_echoPin{ echoPin }, reading{ test_value } { }

void DistanceSensor::init(){
    // TBD
}

Sensor::SensorStates DistanceSensor::poolSensor(){
    return State();
}

const void* DistanceSensor::getReadings() const{
    return reinterpret_cast<const void*>(&reading);
}