#pragma once

#include <cstdint>

namespace Motor{

/*
    pwm - velocity measured output PWM wave
    dir - left(false) or right(true)
*/

using motorSignals = struct{
    uint32_t pwm;
    bool dir;
};


class IDCMotor{
public:
    virtual ~IDCMotor() { }
    virtual void setSpeed(int32_t) noexcept = 0;
    virtual const motorSignals& getCurrentSpeed() const noexcept = 0;
};

}