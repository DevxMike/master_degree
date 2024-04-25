#include "driver.h"
#include "pid.h"

static bool start_experiment = false;
static float angularVelocityLeft = 0.0f;
static float angularVelocityRight = 0.0f;
static uint32_t counter = 0;



void Kernel::main(){
  static unsigned long sensorTimer, logTimer;
  
  while(!jobStack.empty()){
    auto job = jobStack.pop();
    job.cback(job.payload);
  }

    auto readings = odoMgr.getPosition();

    const Motor::MotorManager::angular_array* target = motorManager.TargetAngular();

    int32_t leftV = pidLeft.getOutput(readings.angularLeft, (*target)[0]);
    int32_t rightV = pidRight.getOutput(readings.angularRight, (*target)[1]);
    
    motorManager.setSpeed({{ leftV, rightV }});
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

  commMgr.sendMessage(
    commMgr.createMessage("robot/echo/out", "robot initialized")
  );
}

bool processCMD(const String& s){
    using constants::comm::commands;
    using constants::comm::types::cmd_mapping;

    bool ret = true;

    if(s == commands[cmd_mapping::resetOdo]){
      Kernel::odoMgr.resetActiveOdometry();
    }
    else if(s == commands[cmd_mapping::getOdo]){
      auto tmp = Kernel::odoMgr.getPosition();
      String msg = 
        String("{")
        + String("\"x\" : ")
        + String(tmp.x)
        + String(", \"y\" : ")
        + String(tmp.y)
        + String(", \"theta\" : ")
        + String(tmp.theta)
        + String(" } ");

      Kernel::commMgr.sendMessage(
        Kernel::commMgr.createMessage(
          constants::comm::pubTopicsArray[constants::comm::types::pub_topic_mapping::cmdResponse],
          msg
        )
      );
    }
    else if(s == commands[cmd_mapping::getSensors]){
      using Sensor::SensorMapping;
      using distance_t = Sensor::Encoder::reading_t;
      using Sensor::get_reading;

      auto front_left = get_reading<distance_t>(Kernel::sensorMgr.getSensor(SensorMapping::DST_Front_1));
      auto front = get_reading<distance_t>(Kernel::sensorMgr.getSensor(SensorMapping::DST_Front_2));
      auto front_right = get_reading<distance_t>(Kernel::sensorMgr.getSensor(SensorMapping::DST_Front_3));
      auto rear = get_reading<distance_t>(Kernel::sensorMgr.getSensor(SensorMapping::DST_Rear));

      String msg = 
        String("{ \"front_left\" : ")
        + String(*front_left)
        + String(", \"front\" : ")
        + String(*front)
        + String(", \"front_right\" : ")
        + String(*front_right)
        + String(", \"rear\" : ")
        + String(*rear)
        + String("}");

      Kernel::commMgr.sendMessage(
        Kernel::commMgr.createMessage(
          constants::comm::pubTopicsArray[constants::comm::types::pub_topic_mapping::cmdResponse],
          msg
        )
      );
    }
    else if(s == commands[cmd_mapping::getAll]){
      using Sensor::SensorMapping;
      using distance_t = Sensor::Encoder::reading_t;
      using Sensor::get_reading;

      auto odo = Kernel::odoMgr.getPosition();
      auto front_left = get_reading<distance_t>(Kernel::sensorMgr.getSensor(SensorMapping::DST_Front_1));
      auto front = get_reading<distance_t>(Kernel::sensorMgr.getSensor(SensorMapping::DST_Front_2));
      auto front_right = get_reading<distance_t>(Kernel::sensorMgr.getSensor(SensorMapping::DST_Front_3));
      auto rear = get_reading<distance_t>(Kernel::sensorMgr.getSensor(SensorMapping::DST_Rear));

      String msg = 
        String("{")
        + String("\"x\" : ")
        + String(odo.x)
        + String(", \"y\" : ")
        + String(odo.y)
        + String(", \"theta\" : ")
        + String(odo.theta)
        + String(", \"front_left\" : ")
        + String(*front_left)
        + String(", \"front\" : ")
        + String(*front)
        + String(", \"front_right\" : ")
        + String(*front_right)
        + String(", \"rear\" : ")
        + String(*rear)
        + String("}");

        Kernel::commMgr.sendMessage(
          Kernel::commMgr.createMessage(
            constants::comm::pubTopicsArray[constants::comm::types::pub_topic_mapping::cmdResponse],
            msg
          )
        );
    }
    else if(s == commands[cmd_mapping::halt]){
      Kernel::motorManager.setSpeed(
        {{ 0, 0 }},
        Motor::MotorManager::settingType::setAngularTarget
      );
    }
    else{
      ret = false;
    }

    return ret;
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
