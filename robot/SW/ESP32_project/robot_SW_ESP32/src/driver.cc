#include "driver.h"

void Kernel::main(){
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

void Kernel::MQTTcallback(char* topic, byte* payload, unsigned int len){
  using constants::comm::types::topic_mapping;

  String tmp{ topic };
  constants::comm::types::job_t job;

  if(tmp == constants::comm::topicsArray[topic_mapping::debugInfo]){
    job.cback = [&](const String& s){
      Serial.println("cback 1");
      Serial.println(s);
    };
  }
  else if(tmp == constants::comm::topicsArray[topic_mapping::request]){
    job.cback = [&](const String& s){
      Serial.println("cback 2");
      Serial.println(s);
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
