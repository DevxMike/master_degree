#ifndef global_h
#define global_h

#define WIFI_DEBUG 1
#define MQTT_DEBUG 1
#define MOTOR_DEBUG 0
#define ENCODER_DEBUG 1

#include "Arduino.h"
#include <array>
#include <functional>
#include "DCMotor.h"
#include "stack.h"
#include "pid.h"

namespace constants{
namespace experiment{
    constexpr float samplingTime = 0.005;
}

    constexpr uint8_t defaultResolution = 8;

namespace sensor{
    constexpr uint8_t triggerRear = 13;
    constexpr uint8_t echoRear = 18;
}

namespace comm{
    constexpr const char* SSID = "M&N";
    constexpr const char* WiFiPassword = "+q48uvdETJsT7c";
    constexpr const char* MQTTBroker = "mqtt-dashboard.com";
    constexpr const char* MQTTPass = "";
    constexpr const char* MQTTUname = "";
    
    constexpr uint32_t subscribedTopics = 3;
    constexpr uint32_t jobStackDepth = 10;

    const std::array<String, subscribedTopics> topicsArray{
        { "robot/set/pid", "robot/request", "robot/set/motors" }
    };

namespace types{

    struct job_t{
        std::function<void(const String&)> cback;
        String payload;
    };

    using job_stack_t = custom::stack<constants::comm::types::job_t, constants::comm::jobStackDepth>;

    enum topic_mapping : uint32_t{
        debugInfo = 0,
        request,
        setMotors
    };
}
}

namespace motors{
    constexpr uint32_t impulsesPerRotation = 1920;
    constexpr float inertia_coef = 0.7f;
    constexpr uint8_t enAPin = 12;
    constexpr uint8_t enBPin = 14;
    constexpr uint8_t in1Pin = 27;
    constexpr uint8_t in2Pin = 26;
    constexpr uint8_t in3Pin = 25;
    constexpr uint8_t in4Pin = 33;
    constexpr uint8_t motorAChannel = 0;
    constexpr uint8_t motorBChannel = 2;
    constexpr uint8_t motorPWMResolution = defaultResolution;
    constexpr uint32_t motorPWMFrequency = 1000;
    // "Kp" : 22.75297759, "Ti" : 41.13985303, "Td" : 43.13750817
namespace speed_ctl{
    constexpr float Kp = 22.75297759f;
    constexpr float Ti = 41.13985303f;
    constexpr float Td = 43.13750817f;

    constexpr float vMaxRadiansPerSecond = 16.7f;
}
    
    constexpr Motor::DCMotor::outputs outputsLeft{
        .in1_pin = in1Pin,
        .in2_pin = in2Pin,
        .enable_pin = enAPin,
        .pwm_channel = motorAChannel,
        .frequency = motorPWMFrequency,
        .pwm_resolution = motorPWMResolution
    };

    constexpr Motor::DCMotor::outputs outputsRight{
        .in1_pin = in3Pin,
        .in2_pin = in4Pin,
        .enable_pin = enBPin,
        .pwm_channel = motorBChannel,
        .frequency = motorPWMFrequency,
        .pwm_resolution = motorPWMResolution
    };

    constexpr uint8_t enc1A = 15;
    constexpr uint8_t enc1B = 2;
    constexpr uint8_t enc2A = 0;
    constexpr uint8_t enc2B = 4;

namespace types{
    using motor_pid = pid<float, int32_t, Motor::max_absolute_v, -Motor::max_absolute_v, 1000, -1000>;
}
}

    constexpr float soundSpeed = 0.034; // cm/uS

    // cant make constexpr because String is not constexpr
}

#endif