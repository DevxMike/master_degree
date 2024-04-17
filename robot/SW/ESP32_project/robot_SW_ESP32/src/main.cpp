#include "driver.h"

WiFiClient espClient;

/* motor motion management variables init */
Motor::DCMotor Kernel::motorLeft{ Kernel::mapper, constants::motors::outputsLeft };
Motor::DCMotor Kernel::motorRight{ Kernel::mapper, constants::motors::outputsRight };

// constants::motors::types::motor_pid Kernel::pidRight{ 
//   constants::motors::speed_ctl::Kp, 
//   constants::motors::speed_ctl::Ti, 
//   constants::motors::speed_ctl::Td, 
//   constants::experiment::samplingTime 
// };

// constants::motors::types::motor_pid Kernel::pidLeft{ 
//   constants::motors::speed_ctl::Kp, 
//   constants::motors::speed_ctl::Ti, 
//   constants::motors::speed_ctl::Td, 
//   constants::experiment::samplingTime 
// };

Motor::MotorManager Kernel::motorManager{ 
  Motor::MotorManager::motor_array{{
      &motorLeft, &motorRight
    }},
  Motor::MotorManager::speed_pid_array{{
    constants::motors::types::motor_pid{ constants::motors::speed_ctl::Kp, constants::motors::speed_ctl::Ti, constants::motors::speed_ctl::Td, constants::experiment::samplingTime },
    constants::motors::types::motor_pid{ constants::motors::speed_ctl::Kp, constants::motors::speed_ctl::Ti, constants::motors::speed_ctl::Td, constants::experiment::samplingTime }
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