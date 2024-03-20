#include "../include/IMU.h"

namespace Sensor{

IMU::IMU(uint32_t sdaPin, uint32_t sclPin)noexcept:
    m_sdaPin{ sdaPin }, m_sclPin{ sclPin }, useInt{ false } { 


    }

IMU::IMU(uint32_t sdaPin, uint32_t sclPin, uint32_t intPin) noexcept:
    IMU(sdaPin, sclPin) { 
        useInt = true; 
        m_intPin = intPin;
    } 

void IMU::init() noexcept{
    float i = 0;

    for(auto& x: reading){
        x = i;
        i += 1.0f;
    }
    // TBD
}

Sensor::SensorStates IMU::poolSensor() noexcept{
    // TBD
    return State();
}

const void* IMU::getReadings() const noexcept{
    return reinterpret_cast<const void*>(&reading);
}

};