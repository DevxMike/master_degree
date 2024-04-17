#include "../include/MotorManager.h"
#include "Arduino.h"

namespace Motor{

enum{
    update_speed = 0,
    timeout
};

MotorManager::MotorManager(motor_array&& m, speed_pid_array&& pids, float i) noexcept:
    m_inertiaCoef{ i }, motors{ std::move(m) }, speed_pids{ std::move(pids) } {}

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

    for(auto& s: actual_desired_speed){
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

void MotorManager::setSpeed(const speed_array_percent& speeds, setType t) noexcept{
    // auto d = desired_speed.begin();
    // auto s = speeds.begin();

    // for(; s != speeds.end() && d != desired_speed.end(); s++, d++){
    //     *d = *s;
    // }
    for(std::size_t i = 0; i < motor_num; ++i){
        if(t == setType::setTarget){
            auto tmp = filter_speed(speeds[i]);
            actual_desired_speed[i] = (tmp * 0.01) * constants::motors::speed_ctl::vMaxRadiansPerSecond;
#if MOTOR_DEBUG
    Serial.print("Desired: ");
    Serial.println(actual_desired_speed[i]);
#endif
        }
        else{
            desired_speed[i] = speeds[i];
        }
    }
}

// void MotorManager::setTarget(const speed_array_percent& speeds) noexcept{
//     for(std::size_t i = 0; i < motor_num; ++i){
//         actual_desired_speed[i] = (speeds[i] / 0.01) * constants::motors::speed_ctl::vMaxRadiansPerSecond;
//     }
// }

void MotorManager::poolMotors(const speed_array_angular& actual_angular) noexcept{
    static uint8_t state{ update_speed };
    auto x = m_inertiaCoef;
    auto y = 1.0f - m_inertiaCoef;
    static unsigned long timer;

    switch(state){
    case update_speed:


        for(std::size_t i = 0; i < motor_num; ++i){
            if(std::abs((actual_desired_speed[i] - actual_angular[i]) / (actual_desired_speed[i] + 0.001f)) <= 0.02f){

            }
            else{
                desired_speed[i] = speed_pids[i].getOutput(actual_angular[i], actual_desired_speed[i]);
            }

            current_speed[i] = current_speed[i] * x + desired_speed[i] * y;
            motors[i]->setSpeed(current_speed[i]);
        }
        // timer = millis();
        // state = timeout;
    break;

    case timeout:
        if(millis() - timer > 50){
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

const MotorManager::speed_array_percent* MotorManager::CurrentSpeed() const noexcept{
    return &current_speed;
}

const MotorManager::speed_array_percent* MotorManager::DesiredSpeed() const noexcept{
    return &desired_speed;
}

}