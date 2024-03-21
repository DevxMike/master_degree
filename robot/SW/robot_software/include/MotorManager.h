#pragma once

#include "../DCMotor.h"
#include "../array.h"

namespace Motor{

constexpr uint32_t motor_num = 2;
constexpr float inertia_coef = 0.85f;

class MotorManager{
public:
    void init() noexcept;
    void setSpeed(const custom::array<int32_t, motor_num>& speeds) noexcept;
    MotorManager(float i = inertia_coef) noexcept;
    void InertiaCoef(float i) noexcept;
    auto InertiaCoef() const noexcept;
    void poolMotors() noexcept;
    const auto& CurrentSpeed() const noexcept;
    const auto& DesiredSpeed() const noexcept;

private:
    float m_inertiaCoef;
    custom::array<IDCMotor*, motor_num> motors;
    custom::array<int32_t, motor_num> desired_speed;
    custom::array<int32_t, motor_num> current_speed;
};

}