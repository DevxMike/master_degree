#include <Arduino.h>
#include <array>
#include "CommManager.h"
#include "global_defines.h"
#include "array.h"
#include <initializer_list>
#include "stack.h"
#include "DistanceSensor.h"
#include "MotorManager.h"
#include "ArduinoJson.h"
#include "Encoder.h"

WiFiClient espClient;
custom::stack<constants::comm::types::job_t, constants::comm::jobStackDepth> jobStack;

uint32_t mapper(uint32_t v){
  return static_cast<uint32_t>(
    v * 0.01 * (std::pow(2, constants::motors::motorPWMResolution) - 1)
  );
}

static Motor::DCMotor motorLeft{ mapper, constants::motors::outputsLeft };
static Motor::DCMotor motorRight{ mapper, constants::motors::outputsRight };

static Motor::MotorManager motorManager{ 
  Motor::MotorManager::motor_array{{
      &motorLeft, &motorRight
    }}, 0.7f 
  };

void MQTTcallback(char* topic, byte* payload, unsigned int length){
  using constants::comm::types::topic_mapping;

  String tmp{ topic };
  constants::comm::types::job_t job;

  if(tmp == constants::comm::topicsArray[topic_mapping::debugInfo]){
    job.cback = [&](const String& s){
      Serial.println("cback 1");
      Serial.println(s);

      // motorManager.setSpeed({{ 70, 70 }});
    };
  }
  else if(tmp == constants::comm::topicsArray[topic_mapping::request]){
    job.cback = [&](const String& s){
      Serial.println("cback 2");
      Serial.println(s);

      // motorManager.setSpeed({{ 0, 0 }});
    };
  }
  else if(tmp == constants::comm::topicsArray[topic_mapping::setMotors]){
    job.cback = [&](const String& s){
      StaticJsonBuffer<100> JSONBuffer; 
      JsonObject& parsed = JSONBuffer.parseObject(s);
      
      if(parsed.success()){
        int32_t left = parsed["left"];
        int32_t right = parsed["right"];

        motorManager.setSpeed(Motor::MotorManager::speed_array{{ left, right }});
      }

      Serial.println(s);
    };
  }

  job.payload = String("");

  for(unsigned int i = 0; i < length; ++i){
    job.payload += (char)payload[i];
  }

  jobStack.push(std::move(job));
}



static Comm::MQTT::CommManager<String, constants::comm::subscribedTopics> commMgr(
  Comm::WiFiManager<String>("M&N", "+q48uvdETJsT7c", WiFi), 
  "", 
  "mqtt-dashboard.com", 
  "", 
  espClient, 
  std::array<String, constants::comm::subscribedTopics>{ constants::comm::topicsArray }, 
  MQTTcallback
);

static Sensor::DistanceSensor rear{ constants::sensor::echoRear, constants::sensor::triggerRear };

static Sensor::Encoder encoderLeft{
  constants::motors::enc1A, constants::motors::enc1B,
  Sensor::Encoder::Instance::ENC0
};

static Sensor::Encoder encoderRight{
  constants::motors::enc2A, constants::motors::enc2B,
  Sensor::Encoder::Instance::ENC1
};

void setup() {
  // // put your setup code here, to run once:
  Serial.begin(9600);

  while(commMgr.poolCommManager() != Comm::MQTT::MQTTStatus::NetworkConnected) { }

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

void loop() {
  static unsigned long sensorTimer;

  commMgr.poolCommManager();
  motorManager.poolMotors();
  
  while(!jobStack.empty()){
    auto job = jobStack.pop();
    job.cback(job.payload);
  }

  // rear.poolSensor();

  if(millis() - sensorTimer > 500){
    // using dst_type = Sensor::DistanceSensor::reading_t;
    // auto reading = *(static_cast<const dst_type*>(rear.getReadings()));

    // auto tmp1 = motorManager.DesiredSpeed();
    

    // for(const auto& d: (*tmp1)){
    //   Serial.print("desired: ");
    //   Serial.println(d);
    // }

    // auto tmp2 = motorManager.CurrentSpeed();
    // for(const auto& d: (*tmp1)){
    //   Serial.print("current: ");
    //   Serial.println(d);
    // }

    // Serial.println(reading);
    
    sensorTimer = millis();
    auto reading1 = static_cast<const Sensor::Encoder::reading_t*>(encoderLeft.getReadings());
    auto reading2 = static_cast<const Sensor::Encoder::reading_t*>(encoderRight.getReadings());
#if ENCODER_DEBUG
    Serial.print("enc left: ");
    Serial.println(*reading1);

    Serial.print("enc right: ");
    Serial.println(*reading2);
#endif
  }
}