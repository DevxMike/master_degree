#include "../include/Encoder.h"
#include "Arduino.h"
#include <functional>

#include "global_defines.h"

namespace Sensor{

std::array<Encoder*, encoder_count> Encoder::instances = {};

Encoder::Encoder(uint32_t input1, uint32_t input2, Instance which_enc) noexcept:
    m_input1{ input1 }, m_input2{ input2 }, reading{ 0 }, m_enc_id{ which_enc } { State(Sensor::SensorStates::Measuring); }

void Encoder::init() noexcept{
    pinMode(m_input1, INPUT_PULLUP);
    pinMode(m_input2, INPUT_PULLUP);

    switch(m_enc_id){
        case ENC0:
            instances[ENC0] = this;
            attachInterrupt(m_input1, Encoder::isr0, CHANGE);
        break;

        case ENC1:
            instances[ENC1] = this;
            attachInterrupt(m_input1, Encoder::isr1, CHANGE);
        break;
    }
}

Sensor::SensorStates Encoder::poolSensor() noexcept{
    return State(); // no need for implementation since encoder works in INT mode
}

const void* Encoder::getReadings() const noexcept{
    return reinterpret_cast<const void*>(&reading);
}

void Encoder::reset() noexcept{
    reading = 0;
}

void isr_general(uint8_t in1, uint8_t in2, Encoder::reading_t* reading){
    auto A = digitalRead(in1);
    auto B = digitalRead(in2);

    if ((A == HIGH && B == LOW) || (A == LOW && B == HIGH)){
        (*reading)--;
    }
    else{
        (*reading)++;
    }
}

void Encoder::isr0(){
    auto i = instances[ENC0];
    isr_general(i->m_input1, i->m_input2, &i->reading);
}
void Encoder::isr1(){
    auto i = instances[ENC1];
    isr_general(i->m_input1, i->m_input2, &i->reading);
}


}