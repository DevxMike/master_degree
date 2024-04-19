#include "../include/MotorManager.h"
#include "Arduino.h"
#include "global_defines.h"

namespace Motor{

enum{
    update_speed = 0,
    timeout
};

MotorManager::MotorManager(motor_array&& m, float i) noexcept:
    m_inertiaCoef{ i }, motors{std::move(m)} {}

void MotorManager::init() noexcept{
    for(auto& m : motors){
        (*m).init();
    }

    for(auto& s : target_percent_speed){
        s = 0;
    }

    for(auto& s : current_percent_speed){
        s = 0;
    }

    for(auto& s : target_angular_speed){
        s = 0.0f;
    }
}

static int32_t filter_speed(int32_t s){
    if((s > -max_absolute_v && s < -epsilon_abs) || (s > epsilon_abs && s < max_absolute_v)){
        return s;
    }
    else{
        return 0;
    }
}

void MotorManager::setSpeed(const speed_array& speeds, settingType t) noexcept{
    for(std::size_t i = 0; i < motor_num; ++i){
        if(t == settingType::setAngularTarget){
            int32_t tmp = filter_speed(speeds[i]);
            target_percent_speed[i] = tmp;
            target_angular_speed[i] = (tmp * 0.01f) * constants::motors::speed_ctl::vMaxRadiansPerSecond;
#if MOTOR_DEBUG
    Serial.print("Desired: ");
    Serial.println(target_angular_speed[i]);
#endif
        }
        else if(t == settingType::updatePwm){
            target_percent_speed[i] = speeds[i];
        }
    }
}

void MotorManager::poolMotors() noexcept{
    static uint8_t state{ update_speed };
    auto x = m_inertiaCoef;
    auto y = 1.0f - m_inertiaCoef;
    static unsigned long timer;

    switch(state){
    case update_speed:
        for(std::size_t i = 0; i < motor_num; ++i){
            current_percent_speed[i] = current_percent_speed[i] * x + target_percent_speed[i] * y;
            motors[i]->setSpeed(current_percent_speed[i]);
        }

        // timer = millis();
        // state = timeout;
    break;

    case timeout:
        if(millis() - timer >= 10){
            state = update_speed;
        }
    break;
    }
}

void MotorManager::InertiaCoef(float i) noexcept{
    m_inertiaCoef = i;
}

float MotorManager::InertiaCoef() const noexcept{
    return m_inertiaCoef;
}

const MotorManager::speed_array* MotorManager::CurrentSpeed() const noexcept{
    return &current_percent_speed;
}

const MotorManager::speed_array* MotorManager::DesiredSpeed() const noexcept{
    return &target_percent_speed;
}

const MotorManager::angular_array* MotorManager::TargetAngular() const noexcept{
    return &target_angular_speed;
}

}
