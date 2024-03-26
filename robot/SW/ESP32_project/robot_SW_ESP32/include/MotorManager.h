#ifndef MotorMgr_h
#define MotorMgr_h

#include "DCMotor.h"
#include "array.h"

namespace Motor{

constexpr uint32_t motor_num = 2;
constexpr float inertia_coef = 0.9f;

// SWE_2_030 | SWE_3_050

class MotorManager{
public:
    using motor_array = custom::array<IDCMotor*, motor_num>;
    using speed_array = custom::array<int32_t, motor_num>; 

    void init() noexcept;
    void setSpeed(const speed_array& speeds) noexcept;
    MotorManager(motor_array&& m, float i = inertia_coef) noexcept;
    void InertiaCoef(float i) noexcept;
    float InertiaCoef() const noexcept;
    void poolMotors() noexcept;
    const speed_array* CurrentSpeed() const noexcept;
    const speed_array* DesiredSpeed() const noexcept;

private:
    float m_inertiaCoef;
    custom::array<IDCMotor*, motor_num> motors;
    custom::array<int32_t, motor_num> desired_speed;
    custom::array<int32_t, motor_num> current_speed;
};

}

#endif