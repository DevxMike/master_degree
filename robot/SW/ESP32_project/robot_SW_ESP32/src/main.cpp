#include <Arduino.h>
#include <array>
#include "CommManager.h"
#include "global_defines.h"
#include "array.h"
#include <initializer_list>
#include "stack.h"
#include "DistanceSensor.h"

WiFiClient espClient;
custom::stack<constants::types::job_t, constants::jobStackDepth> jobStack;


void MQTTcallback(char* topic, byte* payload, unsigned int length){
  using constants::types::mapping::topic_mapping;

  String tmp{ topic };
  constants::types::job_t job;

  if(tmp == constants::topicsArray[topic_mapping::debugInfo]){
    job.cback = [&](const String& s){
      Serial.println("cback 1");
      Serial.println(s);
    };
  }
  else if(tmp == constants::topicsArray[topic_mapping::request]){
    job.cback = [&](const String& s){
      Serial.println("cback 2");
      Serial.println(s);
    };
  }

  job.payload = String("");

  for(unsigned int i = 0; i < length; ++i){
    job.payload += (char)payload[i];
  }

  jobStack.push(std::move(job));
}



static Comm::MQTT::CommManager<String, constants::subscribedTopics> commMgr(
  Comm::WiFiManager<String>("M&N", "+q48uvdETJsT7c", WiFi), 
  "", 
  "mqtt-dashboard.com", 
  "", 
  espClient, 
  std::array<String, constants::subscribedTopics>{ constants::topicsArray }, 
  MQTTcallback
);

static Sensor::DistanceSensor rear{ constants::echoRear, constants::triggerRear };

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
}

void loop() {
  static unsigned long sensorTimer;

  commMgr.poolCommManager();
  
  while(!jobStack.empty()){
    auto job = jobStack.pop();
    job.cback(job.payload);
  }

  rear.poolSensor();

  if(millis() - sensorTimer > 500){
    using dst_type = Sensor::DistanceSensor::reading_t;
    auto reading = *(static_cast<const dst_type*>(rear.getReadings()));

    Serial.print("Measured distance rear: ");
    Serial.println(reading);
    
    sensorTimer = millis();
  }
}