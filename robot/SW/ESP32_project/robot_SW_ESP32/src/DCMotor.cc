#include "../include/DCMotor.h"
#include "global_defines.h"

#include "Arduino.h"

namespace Motor{

void MotorMapper(int32_t v, motorSignals& m, map_function_type f) noexcept{
#if MOTOR_DEBUG
    // Serial.print("Desired: ");
    // Serial.println(v);
#endif

    if(v > epsilon_abs){ // rotate left
        v = (v > max_absolute_v)? max_absolute_v : v;
        m.dir = false;
    }
    else if(v < -epsilon_abs){ // rotate right
        v = (v < -max_absolute_v)? max_absolute_v : -v;
        m.dir = true;
    }
    else{
        m.dir = false;
        m.pwm = 0;

        return;
    }

    m.pwm = f(static_cast<uint32_t>(v));
}

DCMotor::DCMotor(map_function_type fmap, const outputs& o) noexcept:
    m_signals{ 0, 0 }, mapper{ fmap }, m_outputs{ o } { }

void DCMotor::setSpeed(int32_t v) noexcept{
    MotorMapper(v, m_signals, mapper);
    
    if(m_signals.pwm != 0){
        if(m_signals.dir){ // rotate right
            digitalWrite(m_outputs.in1_pin, HIGH);
            digitalWrite(m_outputs.in2_pin, LOW);
        }
        else{ // rotate left
            digitalWrite(m_outputs.in1_pin, LOW);
            digitalWrite(m_outputs.in2_pin, HIGH);
        }

#if MOTOR_DEBUG
        // Serial.print(m_signals.dir == false? "left" : "right");
        // Serial.print(", PWM: ");
        // Serial.println(m_signals.pwm);
#endif

        ledcWrite(m_outputs.pwm_channel, m_signals.pwm);
    }
    else{
        digitalWrite(m_outputs.in1_pin, LOW);
        digitalWrite(m_outputs.in2_pin, LOW);
    }
}

void DCMotor::init() noexcept{
    // setup PWM
    ledcSetup(
        m_outputs.pwm_channel,
        m_outputs.frequency,
        m_outputs.pwm_resolution
    );

    ledcAttachPin(
        m_outputs.enable_pin,
        m_outputs.pwm_channel
    );

    // setup outputs
    pinMode(m_outputs.in1_pin, OUTPUT);
    pinMode(m_outputs.in2_pin, OUTPUT);
}

const motorSignals& DCMotor::getCurrentSpeed() const noexcept{
    return m_signals;
} 

}