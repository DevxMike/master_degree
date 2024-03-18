#pragma once

#include "interfaces/ISensor.h"

#include <cstdint>
#include <array>
#include <memory>

namespace Sensor{

constexpr uint8_t numSensors = 7;
using sensorPointer = std::unique_ptr<ISensor>; // define pointer type
                                                // in case of no STL support

enum class SensorMapping : uint8_t{  // indexes in array of sensors and 
                                     // pointers to readings
    LEFT_Encoder,
    RIGHT_Encoder,
    DST_Front_1,
    DST_Front_2,
    DST_Front_3,
    DST_Rear,
    IMU
};

constexpr uint8_t translate(SensorMapping s){ // translate enum into uint
    return static_cast<uint8_t>(s);
}

class SensorManager{
public:
    void init(); // needed due to arduino/platformio framework
    SensorManager();
    void poolSensors();
    const void* getReadings(SensorMapping s);

private:
    std::array<sensorPointer, numSensors> sensors;
};

}