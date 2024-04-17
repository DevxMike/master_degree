#ifndef MotorMgr_h
#define MotorMgr_h

#include "DCMotor.h"
#include "array.h"
#include <array>
#include "pid.h"
#include "global_defines.h"

namespace Motor{

constexpr uint32_t motor_num = 2;
constexpr float inertia_coef = 0.9f;

// SWE_2_030 | SWE_3_050

class MotorManager{
public:
    enum class setType{
        setTarget,
        updatePwm
    };

    using motor_array = std::array<IDCMotor*, motor_num>;
    using speed_array_percent = std::array<int32_t, motor_num>; 
    using speed_array_angular = std::array<float, motor_num>;
    using speed_pid_array = std::array<constants::motors::types::motor_pid, motor_num>;

    void init() noexcept;
    void setSpeed(const speed_array_percent& speeds, setType = setType::updatePwm) noexcept;
    MotorManager(motor_array&& m, speed_pid_array&& pids, float i = inertia_coef) noexcept;
    void InertiaCoef(float i) noexcept;
    float InertiaCoef() const noexcept;
    void poolMotors(const speed_array_angular& actual_angular) noexcept;
    const speed_array_percent* CurrentSpeed() const noexcept;
    const speed_array_percent* DesiredSpeed() const noexcept;

private:
    float m_inertiaCoef;
    motor_array motors;
    speed_array_percent desired_speed; // this field will be modified by PID regulator
    speed_array_percent current_speed;
    speed_array_angular actual_desired_speed;
    speed_pid_array speed_pids;
};

}

#endif