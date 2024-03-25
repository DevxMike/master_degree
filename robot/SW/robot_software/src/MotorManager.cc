#include "../include/MotorManager.h"

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

    for(auto& s : desired_speed){
        s = 0;
    }

    for(auto& s : current_speed){
        s = 0;
    }
}

void MotorManager::setSpeed(const speed_array& speeds) noexcept{
    // auto d = desired_speed.begin();
    // auto s = speeds.begin();

    // for(; s != speeds.end() && d != desired_speed.end(); s++, d++){
    //     *d = *s;
    // }
    for(std::size_t i = 0; i < motor_num; ++i){
        desired_speed[i] = speeds[i];
    }
}

void MotorManager::poolMotors() noexcept{
    static uint8_t state{ update_speed };
    auto x = m_inertiaCoef;
    auto y = 1.0f - m_inertiaCoef;

    switch(state){
    case update_speed:
        for(std::size_t i = 0; i < motor_num; ++i){
            current_speed[i] = current_speed[i] * x + desired_speed[i] * y;
            motors[i]->setSpeed(current_speed[i]);
        }

        state = timeout;
    break;

    case timeout:
        // TBD
        state = update_speed;
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
    return &current_speed;
}

const MotorManager::speed_array* MotorManager::DesiredSpeed() const noexcept{
    return &desired_speed;
}

}