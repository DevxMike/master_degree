#include "../include/DCMotor.h"

namespace Motor{

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