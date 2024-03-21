#pragma once

#include "interfaces/IDCMotor.h"

namespace Motor{
/*
    v - normalized V of the motor between -100 and 100,
    m - struct of motorSignals values to be mapped to PWM
*/

constexpr int32_t max_absolute_v = 50;
constexpr int32_t epsilon_abs = 10;

using map_function_type = uint32_t (*)(uint32_t);

constexpr void MotorMapper(int32_t v, motorSignals& m, map_function_type f) noexcept{
    if(v > epsilon_abs){ // rotate left
        v = (v > max_absolute_v)? max_absolute_v : v;
        m.dir = false;
    }
    else if(v < -epsilon_abs){ // rotate right
        v = (v < -max_absolute_v)? max_absolute_v : -v;
        m.dir = true;
    }
    else{
        m.dir = false;
        m.pwm = 0;

        return;
    }

    m.pwm = f(static_cast<uint32_t>(v));
}

class DCMotor : public IDCMotor{
public:
    struct outputs{
        uint32_t in1;
        uint32_t in2;
        uint32_t enable;
    };
    void init() noexcept override;
    DCMotor(map_function_type fmap, const outputs& o) noexcept;
    void setSpeed(int32_t) noexcept override;
    const motorSignals& getCurrentSpeed() const noexcept;
private:
    motorSignals m_signals;
    map_function_type mapper;
    outputs m_outputs;
};

}