#include "DistanceSensor.h"
#include "Arduino.h"
#include "global_defines.h"

namespace Sensor{

constexpr unsigned long distanceSensorTimeout = 50;

DistanceSensor::DistanceSensor(uint32_t echoPin, uint32_t trigPin) noexcept :
    m_trigPin{ trigPin }, m_echoPin{ echoPin }, reading{ test_value } { State(SensorStates::SensorInit); }

void DistanceSensor::init() noexcept {
    pinMode(m_trigPin, OUTPUT);
    pinMode(m_echoPin, INPUT);
    State(SensorStates::Timeout);
}

Sensor::SensorStates DistanceSensor::poolSensor() noexcept {
    static unsigned long timer{ 0 };
    float duration;

    switch(State()){
        case SensorStates::Measuring:
            digitalWrite(m_trigPin, LOW);
            delayMicroseconds(2);

            digitalWrite(m_trigPin, HIGH);
            delayMicroseconds(10);

            digitalWrite(m_trigPin, LOW);

            duration = pulseIn(m_echoPin, HIGH);
            reading = static_cast<reading_t>(0.9 * reading + duration * constants::soundSpeed * 0.1) ;
            
            timer = millis();
            State(SensorStates::Timeout);
            #if SENSOR_DEBUG
            Serial.print("sensor reading: ");
            Serial.println(reading);
            #endif
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