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

};

}