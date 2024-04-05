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

static Comm::MQTT::CommManager<String> commMgr(
  Comm::WiFiManager<String>("M&N", "+q48uvdETJsT7c", WiFi), 
  "", "mqtt-dashboard.com", "", espClient
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
  Serial.println("Connected to MQTT broker");
  commMgr.sendMessage(
    Comm::MQTT::CommManager<String>::createMessage("robot/debug", "hello from robot")
  );
#endif

}

void loop() {
  commMgr.poolCommManager();
}