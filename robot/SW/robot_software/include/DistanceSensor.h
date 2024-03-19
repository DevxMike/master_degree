#pragma once

#include <cstdint>
#include "../include/interfaces/ISensor.h"

namespace Sensor{
constexpr uint32_t test_value = 70;

class DistanceSensor : public Sensor::ISensor{
public:
    using reading_t = uint32_t;

    ~DistanceSensor() { }
    DistanceSensor(uint32_t echoPin, uint32_t trigPin);
    void init() override;
    Sensor::SensorStates poolSensor() override;
    const void* getReadings() const;
private:
    reading_t reading;
    uint32_t m_trigPin;
    uint32_t m_echoPin;
};

}