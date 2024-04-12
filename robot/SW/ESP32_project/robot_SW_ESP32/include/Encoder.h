#ifndef Encoder_h
#define Encoder_h

#include <cstdint>
#include "interfaces/ISensor.h"
#include <array>
namespace Sensor{

constexpr uint32_t test_value_enc = 100;
constexpr uint8_t encoder_count = 2;

// SWE_2_010 | SWE_2_050

class Encoder : public ISensor{
public:
    enum Instance : uint8_t{
        ENC0 = 0,
        ENC1
    };

    using reading_t = int32_t;
    Encoder() noexcept { }
    Encoder(uint32_t input1, uint32_t input2, Instance which_enc) noexcept;
    void init() noexcept override;
    Sensor::SensorStates poolSensor() noexcept override;
    const void* getReadings() const noexcept;
    void reset() noexcept;
    

private:
    static std::array<Encoder*, encoder_count> instances;

    static void isr0();
    static void isr1();

    reading_t reading;
    uint8_t m_input1;
    uint8_t m_input2;
    uint8_t m_enc_id;
};

}

#endif