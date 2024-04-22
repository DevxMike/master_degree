#ifndef IMU_h
#define IMU_h

#include "interfaces/ISensor.h"
#include "array.h"
#include <array>

namespace Sensor{

constexpr uint32_t num_dimensions = 9;

// SWE_3_010 | SWE_2_010

class IMU : public ISensor{
public: 
    using reading_t = std::array<float, num_dimensions>;

    IMU(uint32_t sdaPin, uint32_t sclPin) noexcept;
    IMU(uint32_t sdaPin, uint32_t sclPin, uint32_t intPin) noexcept; 
    void init() noexcept override; 
    SensorStates poolSensor() noexcept override;
    const void* getReadings() const noexcept override;
    bool useInterrupts() const noexcept { return useInt; }

private:
    reading_t reading;
    uint32_t m_sdaPin;
    uint32_t m_sclPin;
    uint32_t m_intPin;
    bool useInt;
};

}

#endif