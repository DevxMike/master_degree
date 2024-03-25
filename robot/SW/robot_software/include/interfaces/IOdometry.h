#pragma once

#include "../SensorManager.h"

namespace Sensor{

class IOdometry{
public:
    virtual ~IOdometry() { }
    virtual void poolOdometry(SensorManager& s) noexcept = 0;
    virtual void resetOdometry() noexcept = 0;
    virtual void init() noexcept;
};

}