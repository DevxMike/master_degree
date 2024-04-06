#include "DistanceSensor.h"
#include "Arduino.h"

namespace Sensor{

constexpr unsigned long distanceSensorTimeout = 100;

DistanceSensor::DistanceSensor(uint32_t echoPin, uint32_t trigPin) noexcept :
    m_trigPin{ trigPin }, m_echoPin{ echoPin }, reading{ test_value } { State(SensorStates::SensorInit); }

void DistanceSensor::init() noexcept {
    pinMode(m_trigPin, OUTPUT);
    pinMode(m_echoPin, INPUT);
    State(SensorStates::Timeout);
}

Sensor::SensorStates DistanceSensor::poolSensor() noexcept {
    static unsigned long timer{ 0 };

    switch(State()){
        case SensorStates::Measuring:
            // logic TBD

            timer = millis();
            State(SensorStates::Timeout);
        break;

        case SensorStates::Timeout:
            if(millis() - timer > distanceSensorTimeout){
                State(SensorStates::Measuring);
            }
        break;
    }

    return State();
}

const void* DistanceSensor::getReadings() const noexcept{
    return reinterpret_cast<const void*>(&reading);
}

}