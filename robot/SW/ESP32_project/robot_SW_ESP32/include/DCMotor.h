#ifndef DCMotor_h
#define DCMotor_h
#include "interfaces/IDCMotor.h"

namespace Motor{


constexpr int32_t max_absolute_v = 90;
constexpr int32_t epsilon_abs = 10;

using map_function_type = uint32_t (*)(uint32_t);
/*
    v - normalized V of the motor between -100 and 100,
    m - struct of motorSignals values to be mapped to PWM
    f - mapper function type (depending on PWM resoultion)
*/
void MotorMapper(int32_t v, motorSignals& m, map_function_type f) noexcept;

class DCMotor : public IDCMotor{
public:
    struct outputs{
        uint8_t in1_pin;
        uint8_t in2_pin;
        uint8_t enable_pin;
        uint8_t pwm_channel;
        uint32_t frequency;
        uint8_t pwm_resolution;
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

#endif