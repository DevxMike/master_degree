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
    const int32_t* reading1 = Sensor::get_reading<Sensor::Encoder::reading_t>(encoderLeft);
    const int32_t* reading2 = Sensor::get_reading<Sensor::Encoder::reading_t>(encoderRight);

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

    String payload = 
      String("{ \"id\" : ") 
      + String(counter++) 
      + String(", \"target\" :") 
      + String((*target)[0]) 
      + String(", \"actual\" : ")
      + String(angularVelocityLeft) 
      + String(" }");

    commMgr.sendMessage(
      Comm::MQTT::CommManager<String, constants::comm::subscribedTopics>::createMessage("robot/pid/log", payload)
    );

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

  motorManager.init();
  sensorMgr.init();
}

bool processCMD(const String& s){
    using constants::comm::commands;
    using constants::comm::types::cmd_mapping;

    if(s == commands[cmd_mapping::resetOdo]){
      // reset odo

      return true;
    }
    else if(s == commands[cmd_mapping::getOdo]){
      // send position

      return true;
    }
    else if(s == commands[cmd_mapping::getSensors]){
      // send sensors

      return true;
    }
    else if(s == commands[cmd_mapping::getAll]){
      // send position and sensors

      return true;
    }
    else if(s == commands[cmd_mapping::halt]){
      Kernel::motorManager.setSpeed(
        {{ 0, 0 }},
        Motor::MotorManager::settingType::setAngularTarget
      );

      return true;
    }

    return false;
}

void Kernel::MQTTcallback(char* topic, byte* payload, unsigned int len){
  using constants::comm::types::sub_topic_mapping;
  using constants::comm::types::pub_topic_mapping;
  using constants::comm::pubTopicsArray;
  using constants::comm::subTopicsArray;

  String tmp{ topic };
  constants::comm::types::job_t job;

  if(tmp == subTopicsArray[sub_topic_mapping::setMotors]){
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
  else if(tmp == subTopicsArray[sub_topic_mapping::echoIn]){
    job.cback = [&](const String& s){
      auto msg = Comm::MQTT::CommManager<String, 1>::createMessage(
        pubTopicsArray[pub_topic_mapping::echoOut], s
      );

      Kernel::commMgr.sendMessage(std::move(msg));
    };
  }
  else if(tmp == subTopicsArray[sub_topic_mapping::cmdIn]){
    job.cback = [&](const String& s){
      if(!processCMD(s)){
        auto msg = Comm::MQTT::CommManager<String, 1>::createMessage(
          pubTopicsArray[pub_topic_mapping::cmdResponse], String("cmd not supported")
        );

        Kernel::commMgr.sendMessage(std::move(msg));
      }
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
