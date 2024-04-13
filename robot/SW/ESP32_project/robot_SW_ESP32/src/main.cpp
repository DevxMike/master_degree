#include "driver.h"

WiFiClient espClient;
constants::comm::types::job_stack_t Kernel::jobStack;

/* motor motion management variables init */
Motor::DCMotor Kernel::motorLeft{ Kernel::mapper, constants::motors::outputsLeft };
Motor::DCMotor Kernel::motorRight{ Kernel::mapper, constants::motors::outputsRight };

Motor::MotorManager Kernel::motorManager{ 
  Motor::MotorManager::motor_array{{
      &motorLeft, &motorRight
    }},
    constants::motors::inertia_coef
};

Comm::MQTT::CommManager<String, constants::comm::subscribedTopics> Kernel::commMgr(
  Comm::WiFiManager<String>(constants::comm::SSID, constants::comm::WiFiPassword, WiFi), 
  constants::comm::MQTTUname, 
  constants::comm::MQTTBroker, 
  constants::comm::MQTTPass, 
  espClient, 
  std::array<String, constants::comm::subscribedTopics>{ constants::comm::topicsArray }, 
  Kernel::MQTTcallback
);

Sensor::DistanceSensor Kernel::rear{ constants::sensor::echoRear, constants::sensor::triggerRear };

Sensor::Encoder Kernel::encoderLeft{
  constants::motors::enc1A, constants::motors::enc1B,
  Sensor::Encoder::Instance::ENC0
};

Sensor::Encoder Kernel::encoderRight{
  constants::motors::enc2A, constants::motors::enc2B,
  Sensor::Encoder::Instance::ENC1
};

void setup() {
  Kernel::init();
}

void loop() {
  Kernel::main();
}