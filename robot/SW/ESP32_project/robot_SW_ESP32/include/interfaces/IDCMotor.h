#ifndef IDC_MOTOR_h
#define IDC_MOTOR_h

#include <cstdint>

namespace Motor{

/*
    pwm - velocity measured output PWM wave
    dir - left(false) or right(true)
*/

struct motorSignals{
    uint32_t pwm;
    bool dir;
};


class IDCMotor{
public:
    virtual void init() noexcept = 0;
    virtual ~IDCMotor() { }
    virtual void setSpeed(int32_t) noexcept = 0;
    virtual const motorSignals& getCurrentSpeed() const noexcept = 0;
};

}

#endif