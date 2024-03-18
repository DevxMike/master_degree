#pragma once

namespace Sensor{

enum class States{
    SensorInit,
    Measuring,
    Timeout
};

struct Readings;

class ISensor{
public:
    virtual ~ISensor() = 0;
    virtual States poolSensor() = 0;
    virtual const Readings& getReadings() const = 0;
};

} 