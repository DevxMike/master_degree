#pragma once

#include <cstdint>
#include "interfaces/ISensor.h"

namespace Sensor{

constexpr uint32_t test_value_enc = 100;

// SWE_2_010 | SWE_2_050

class Encoder : public ISensor{
public:
    using reading_t = uint32_t;

    Encoder() noexcept { }
    Encoder(uint32_t input1, uint32_t input2) noexcept;
    void init() noexcept override;
    Sensor::SensorStates poolSensor() noexcept override;
    const void* getReadings() const noexcept;
    void reset() noexcept;
private:
    reading_t reading;
    uint32_t m_input1;
    uint32_t m_input2;
};

}