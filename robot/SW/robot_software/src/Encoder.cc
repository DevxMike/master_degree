#include "../include/Encoder.h"

namespace Sensor{

Encoder::Encoder(uint32_t input1, uint32_t input2) noexcept:
    m_input1{ input1 }, m_input2{ input2 }, reading{ test_value_enc } { }

void Encoder::init() noexcept{
    // TBD
}

Sensor::SensorStates Encoder::poolSensor() noexcept{
    // TBD
    return State();
}

const void* Encoder::getReadings() const noexcept{
    return reinterpret_cast<const void*>(&reading);
}

void Encoder::reset() noexcept{
    reading = 0;
}


}