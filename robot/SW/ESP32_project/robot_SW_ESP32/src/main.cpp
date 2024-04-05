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

static Comm::MQTT::CommManager<String> commMgr(
  Comm::WiFiManager<String>("M&N", "+q48uvdETJsT7c", WiFi), 
  "dev_id", "broker", "pass"
);


void setup() {
  // // put your setup code here, to run once:
  Serial.begin(9600);

  while(commMgr.poolNetwork() != Comm::NetworkStatus::Connected) { }

#if WIFI_DEBUG
  Serial.println("Connected to WiFi");
#endif

}

void loop() {
  // put your main code here, to run repeatedly:
}