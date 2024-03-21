#include "../MotorManager.h"

namespace Motor{

enum{
    update_speed = 0,
    timeout
};

MotorManager::MotorManager(const motor_array& m, float i) noexcept:
    m_inertiaCoef{ i } {
        for(std::size_t i = 0; i < motor_num; ++i){
            motors[i] = m[i];
        }
    }

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
    for(std::size_t i = 0; i < motor_num; ++i){
        current_speed[i] = speeds[i];
    }
}

void MotorManager::poolMotors() noexcept{
    static uint8_t state{ update_speed };

    switch(state){
    case update_speed:
        auto x = m_inertiaCoef;
        auto y = 1.0f - m_inertiaCoef;

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

auto MotorManager::InertiaCoef() const noexcept{
    return m_inertiaCoef;
}

const auto& MotorManager::CurrentSpeed() const noexcept{
    return current_speed;
}

const auto& MotorManager::DesiredSpeed() const noexcept{
    return current_speed;
}

}