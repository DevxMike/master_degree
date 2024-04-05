#include <Arduino.h>
#include <array>
#include "CommManager.h"
#include "global_defines.h"
#include "array.h"
#include <initializer_list>
// put function declarations here:

// static Comm::WiFiManager<String> wifiMgr(
//   "M&N", "+q48uvdETJsT7c", WiFi
// );
WiFiClient espClient;

void MQTTcallback(char* topic, byte* payload, unsigned int length){
  Serial.print("Received message on topic: \"");
  Serial.print(topic);
  Serial.println("\"");

  for(unsigned int i = 0; i < length; ++i){
    Serial.print((char)payload[i]);
  }

  Serial.println("");
}

static Comm::MQTT::CommManager<String, 1> commMgr(
  Comm::WiFiManager<String>("M&N", "+q48uvdETJsT7c", WiFi), 
  "", "mqtt-dashboard.com", "", espClient, std::array<String, 1>{{ "robot/debug/input" }} , MQTTcallback
);


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

}

void loop() {
  commMgr.poolCommManager();
}