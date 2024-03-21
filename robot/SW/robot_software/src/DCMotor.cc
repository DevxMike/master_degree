#include "../include/DCMotor.h"

namespace Motor{

DCMotor::DCMotor(map_function_type fmap) noexcept:
    m_signals{ 0, 0 }, mapper{ fmap } { }

void DCMotor::setSpeed(int32_t v) noexcept{
    MotorMapper(v, m_signals, mapper);
    // TBD low-level functions
}

const motorSignals& DCMotor::getCurrentSpeed() const noexcept{
    return m_signals;
} 

}