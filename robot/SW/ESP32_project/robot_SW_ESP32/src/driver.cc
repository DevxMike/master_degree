#include "driver.h"
#include "pid.h"

static float angularVelocityLeft = 0.0f;
static float angularVelocityRight = 0.0f;

constexpr float getAng(int32_t numImpulses){
      return 
        (1.0f/constants::experiment::samplingTime)
        * (static_cast<float>(numImpulses)/constants::motors::impulsesPerRotation)
        * 2
        * PI;
    };

void Kernel::main(){
      static unsigned long sensorTimer, logTimer;

  commMgr.poolCommManager();

  // rear.poolSensor();

  if(millis() - sensorTimer >= constants::experiment::samplingTimeMs){
    auto leftV = motorManager.DesiredSpeed()->at(0);
    sensorTimer = millis();
    auto reading1 = static_cast<const Sensor::Encoder::reading_t*>(encoderLeft.getReadings());
    auto reading2 = static_cast<const Sensor::Encoder::reading_t*>(encoderRight.getReadings());

    auto angLeft = getAng(*reading1);
    auto angRight = getAng(*reading2);

    angularVelocityLeft = angularVelocityLeft * 0.7 + angLeft * 0.3;
    angularVelocityRight = angularVelocityRight * 0.7 + angRight * 0.3;

    motorManager.poolMotors({{angularVelocityLeft, angularVelocityRight}});

    

    if(millis() - logTimer > 500){
      Serial.print("Angular left: ");
      Serial.print(angularVelocityLeft);
      Serial.print(", Angular right: ");
      Serial.println(angularVelocityRight);
      logTimer = millis();
    }

    // String payload = 
    //   "{ \"id\" : " 
    //   + String(counter++) 
    //   + ", \"target\" :" 
    //   + String(angularVelocityLeft) 
    //   + ", \"actual\" : "
    //   + String(angularVelocityRight) 
    //   + " }";

    // commMgr.sendMessage(
    //   Comm::MQTT::CommManager<String, constants::comm::subscribedTopics>::createMessage("robot/pid/log", payload)
    // );

    encoderLeft.reset();
    encoderRight.reset();
  }
}

void Kernel::init(){
    Serial.begin(9600);

    while(commMgr.poolCommManager() != Comm::MQTT::MQTTStatus::NetworkConnected);

    #if WIFI_DEBUG
  Serial.println("Connected to WiFi");
#endif

  while(commMgr.poolCommManager() != Comm::MQTT::MQTTStatus::BrokerConnected) { }

#if MQTT_DEBUG
  #define CREATE_MESSAGE(i) \
    Comm::MQTT::CommManager<String, 1>::createMessage("robot/debug", String("hello from robot ") + char('0' + i))

  Serial.println("Connected to MQTT broker");
  for(int i = 0; i < 10; ++i) commMgr.sendMessage(
    CREATE_MESSAGE(i)
  );
#endif

  rear.init();
  encoderLeft.init();
  encoderRight.init();

  motorManager.init();
}

void Kernel::MQTTcallback(char* topic, byte* _payload, unsigned int len){
  using constants::comm::types::topic_mapping;

  String tmp{ topic };
  String payload;


  for(unsigned int i = 0; i < len; ++i){
    payload += (char)_payload[i];
  }
  


  if(tmp == constants::comm::topicsArray[topic_mapping::debugInfo]){
      StaticJsonBuffer<20> JSONBuffer; 
      JsonObject& parsed = JSONBuffer.parseObject(payload);
      if(parsed.success()){

      }
  }
  else if(tmp == constants::comm::topicsArray[topic_mapping::request]){
      Serial.println("cback 2");
  }
  else if(tmp == constants::comm::topicsArray[topic_mapping::setMotors]){
      StaticJsonBuffer<100> JSONBuffer; 
      JsonObject& parsed = JSONBuffer.parseObject(payload);
      
      if(parsed.success()){
        int32_t left = parsed["left"];
        int32_t right = parsed["right"];
        angularVelocityLeft = angularVelocityRight = 0.0f;
        motorManager.setSpeed({{ left, right }}, Motor::MotorManager::setType::setTarget);
      }
  }
}

uint32_t Kernel::mapper(uint32_t v){
  return static_cast<uint32_t>(
    v * 0.01 * (std::pow(2, constants::motors::motorPWMResolution) - 1)
  );
}
