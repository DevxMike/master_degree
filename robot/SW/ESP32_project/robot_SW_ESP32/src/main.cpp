#include "driver.h"

WiFiClient espClient;
constants::comm::types::job_stack_t Kernel::jobStack;

/* motor motion management variables init */
Motor::DCMotor Kernel::motorLeft{ Kernel::mapper, constants::motors::outputsLeft };
Motor::DCMotor Kernel::motorRight{ Kernel::mapper, constants::motors::outputsRight };

constants::motors::types::motor_pid Kernel::pid{ 
  constants::motors::speed_ctl::Kp, 
  constants::motors::speed_ctl::Ti, 
  constants::motors::speed_ctl::Td 
};

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

void CommTask(void* p){
  while(1){
    Kernel::commMgr.poolCommManager();
    TASK_DELAY_MS(3);
  }
}

void MotorTask(void* p){
  while(1){
    Kernel::motorManager.poolMotors();
    TASK_DELAY_MS(5);
  }
}

void MainTask(void* p){
  while(1){
    Kernel::main();
    TASK_DELAY_MS(5);
  }
}

static StaticTask_t xCommTaskBuffer;
static StaticTask_t xMotorTaskBuffer;
static StaticTask_t xMainTaskBuffer;

static StackType_t xCommStack[ constants::defaultStackSize * 4];
static StackType_t xMotorStack[ constants::defaultStackSize * 4 ];
static StackType_t xMainStack[ constants::defaultStackSize * 13 ];

void setup() {
  Kernel::init();
  
  xTaskCreateStatic(
    CommTask,
    "comm",
    constants::defaultStackSize * 4,
    NULL,
    2,
    xCommStack,
    &xCommTaskBuffer
  );

  xTaskCreateStaticPinnedToCore(
    MotorTask,
    "motor",
    constants::defaultStackSize * 4,
    NULL,
    3,
    xMotorStack,
    &xMotorTaskBuffer,
    1
  );

  xTaskCreateStaticPinnedToCore(
    MainTask,
    "main",
    constants::defaultStackSize * 13,
    NULL,
    2,
    xMainStack,
    &xMainTaskBuffer,
    1
  );
}

void loop() {

}