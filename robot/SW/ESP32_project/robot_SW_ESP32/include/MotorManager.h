#ifndef MotorMgr_h
#define MotorMgr_h

#include "DCMotor.h"
#include "array.h"
#include <array>

namespace Motor{

constexpr uint32_t motor_num = 2;
constexpr float inertia_coef = 0.9f;

// SWE_2_030 | SWE_3_050

class MotorManager{
public:
    enum class settingType{
        setAngularTarget,
        updatePwm
    };

    using motor_array = std::array<IDCMotor*, motor_num>;
    using speed_array = std::array<int32_t, motor_num>; 
    using angular_array = std::array<float, motor_num>;

    void init() noexcept;
    void setSpeed(const speed_array& speeds, settingType t = settingType::updatePwm) noexcept;
    MotorManager(motor_array&& m, float i = inertia_coef) noexcept;
    void InertiaCoef(float i) noexcept;
    float InertiaCoef() const noexcept;
    void poolMotors() noexcept;
    const speed_array* CurrentSpeed() const noexcept;
    const speed_array* DesiredSpeed() const noexcept;
    const angular_array* TargetAngular() const noexcept;

private:
    float m_inertiaCoef;
    motor_array motors;
    speed_array target_percent_speed;
    speed_array current_percent_speed;
    angular_array target_angular_speed;
};

}

#endif