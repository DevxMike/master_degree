#include "../include/DCMotor.h"



namespace Motor{

void MotorMapper(int32_t v, motorSignals& m, map_function_type f) noexcept{
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

DCMotor::DCMotor(map_function_type fmap, const outputs& o) noexcept:
    m_signals{ 0, 0 }, mapper{ fmap }, m_outputs{ 0 } { }

void DCMotor::setSpeed(int32_t v) noexcept{
    MotorMapper(v, m_signals, mapper);
    // TBD low-level functions
}

void DCMotor::init() noexcept{
    // TBD
}

const motorSignals& DCMotor::getCurrentSpeed() const noexcept{
    return m_signals;
} 

}