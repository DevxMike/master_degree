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
    virtual void State(SensorStates s) { state = s; }
    virtual SensorStates State() const { return state; }     
    virtual void init() = 0; // needed due to arduino/platformio framework
    virtual ~ISensor() {};
    virtual SensorStates poolSensor() = 0;
    virtual const void* getReadings() const = 0;
private:
    SensorStates state;
};

} 