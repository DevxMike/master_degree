#include "driver.h"
#include "pid.h"

static bool start_experiment = false;
static float angularVelocityLeft = 0.0f;
static float angularVelocityRight = 0.0f;
static uint32_t counter = 0;

constexpr float getAng(int32_t numImpulses){
      return 
        (1.0f/constants::experiment::samplingTime)
        * (static_cast<float>(numImpulses)/constants::motors::impulsesPerRotation)
        * 2
        * PI;
    }

void Kernel::main(){
  static unsigned long sensorTimer, logTimer;
  
  while(!jobStack.empty()){
    auto job = jobStack.pop();
    job.cback(job.payload);
  }

  // rear.poolSensor();

    // int32_t leftV = motorManager.DesiredSpeed()->at(0);
    // sensorTimer = millis();
    const int32_t* reading1 = static_cast<const Sensor::Encoder::reading_t*>(encoderLeft.getReadings());
    const int32_t* reading2 = static_cast<const Sensor::Encoder::reading_t*>(encoderRight.getReadings());

    float angLeft = getAng(*reading1);
    float angRight = getAng(*reading2);

    angularVelocityLeft = angularVelocityLeft * 0.7 + angLeft * 0.3;
    angularVelocityRight = angularVelocityRight * 0.7 + angRight * 0.3;

    // angularVelocityLeft = getAng(*reading1);
    // angularVelocityRight = getAng(*reading2);

    // int32_t rightV;

    // if(leftV == 0){
    //   rightV = 0;
    // }
    // else{
    //   rightV = pid.getOutput(angularVelocityRight, angularVelocityLeft);
    // }

    // motorManager.setSpeed({{ leftV, rightV }});
    const Motor::MotorManager::angular_array* target = motorManager.TargetAngular();

    int32_t leftV = pidLeft.getOutput(angularVelocityLeft, (*target)[0]);
    int32_t rightV = pidRight.getOutput(angularVelocityRight, (*target)[1]);
    
    motorManager.setSpeed({{ leftV, rightV }});

    if(millis() - logTimer > 500){
      Serial.print("Angular left: ");
      Serial.print(angularVelocityLeft);
      Serial.print(", Angular right: ");
      Serial.println(angularVelocityRight);
      logTimer = millis();
    }

    // String payload = 
    //   String("{ \"id\" : ") 
    //   + String(counter++) 
    //   + String(", \"target\" :") 
    //   + String((*target)[1]) 
    //   + String(", \"actual\" : ")
    //   + String(angularVelocityRight) 
    //   + String(" }");

    // commMgr.sendMessage(
    //   Comm::MQTT::CommManager<String, constants::comm::subscribedTopics>::createMessage("robot/pid/log", payload)
    // );

    encoderLeft.reset();
    encoderRight.reset();
}

void Kernel::init(){
    Serial.begin(115200);

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

void Kernel::MQTTcallback(char* topic, byte* payload, unsigned int len){
  using constants::comm::types::topic_mapping;

  String tmp{ topic };
  constants::comm::types::job_t job;

  if(tmp == constants::comm::topicsArray[topic_mapping::debugInfo]){
    job.cback = [&](const String& s){
      StaticJsonBuffer<200> JSONBuffer; 
      JsonObject& parsed = JSONBuffer.parseObject(s);
      if(parsed.success()){
         float Kp = parsed["Kp"];
         float Ti = parsed["Ti"];
         float Td = parsed["Td"];

         Serial.println("Changing pid coeffs");

         pidRight.reinit(Kp, Ti, Td);
      }
    };
  }
  else if(tmp == constants::comm::topicsArray[topic_mapping::request]){
    job.cback = [&](const String& s){
      Serial.println("cback 2");
      Serial.println(s);
      start_experiment = false;
      motorManager.setSpeed(Motor::MotorManager::speed_array{{ 0, 0 }}, Motor::MotorManager::settingType::setAngularTarget);
    };
  }
  else if(tmp == constants::comm::topicsArray[topic_mapping::setMotors]){
    job.cback = [&](const String& s){
      StaticJsonBuffer<100> JSONBuffer; 
      JsonObject& parsed = JSONBuffer.parseObject(s);
      
      if(parsed.success()){
        int32_t left = parsed["left"];
        int32_t right = parsed["right"];
        start_experiment = true;
        counter = 0;
        angularVelocityLeft = angularVelocityRight = 0.0f;
        motorManager.setSpeed(Motor::MotorManager::speed_array{{ left, right }}, Motor::MotorManager::settingType::setAngularTarget);
      }

      Serial.println(s);
    };
  }

  job.payload = String("");

  for(unsigned int i = 0; i < len; ++i){
    job.payload += (char)payload[i];
  }

  jobStack.push(std::move(job));
}

uint32_t Kernel::mapper(uint32_t v){
  return static_cast<uint32_t>(
    v * 0.01 * (std::pow(2, constants::motors::motorPWMResolution) - 1)
  );
}
