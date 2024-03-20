#pragma once

namespace Sensor{

enum class SensorStates{
    SensorInit,
    Measuring,
    Timeout
};

struct Readings;

class ISensor{
public:
    virtual void State(SensorStates s) noexcept { state = s; }
    virtual SensorStates State() const noexcept { return state; }     
    virtual void init() noexcept = 0; // needed due to arduino/platformio framework
    virtual ~ISensor() noexcept { }
    virtual SensorStates poolSensor() noexcept = 0;
    virtual const void* getReadings() const noexcept = 0;
private:
    SensorStates state;
};

} 