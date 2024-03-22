#pragma once

#include <cstdint>
#include "../include/interfaces/ISensor.h"

namespace Sensor{
constexpr uint32_t test_value = 70;

// SWE_3_010 | SWE_2_010

class DistanceSensor : public Sensor::ISensor{
public:
    using reading_t = uint32_t;

    ~DistanceSensor() noexcept { }
    DistanceSensor(uint32_t echoPin, uint32_t trigPin) noexcept;
    void init() noexcept override;
    Sensor::SensorStates poolSensor() noexcept override;
    const void* getReadings() const noexcept;
private:
    reading_t reading;
    uint32_t m_trigPin;
    uint32_t m_echoPin;
};

}