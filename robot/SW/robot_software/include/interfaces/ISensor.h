#pragma once

namespace Sensor{

enum class SensorStates{
    SensorInit,
    Measuring,
    Timeout
};

struct Readings;

class ISensor{
    SensorStates state;

public:
    virtual void init() = 0; // needed due to arduino/platformio framework
    virtual ~ISensor() = 0;
    virtual SensorStates poolSensor() = 0;
    virtual const void* getReadings() const = 0;
};

} 