#include "DistanceSensor.h"

namespace Sensor{

DistanceSensor::DistanceSensor(uint32_t echoPin, uint32_t trigPin) noexcept :
    m_trigPin{ trigPin }, m_echoPin{ echoPin }, reading{ test_value } { }

void DistanceSensor::init() noexcept {
    // TBD
}

Sensor::SensorStates DistanceSensor::poolSensor() noexcept {
    // TBD
    return State();
}

const void* DistanceSensor::getReadings() const noexcept{
    return reinterpret_cast<const void*>(&reading);
}

}