#include <Arduino.h>
#include <array>
#include "WiFiManager.h"
#include "global_defines.h"
#include "array.h"
#include <initializer_list>
// put function declarations here:

static Comm::WiFiManager<String> wifiMgr(
  "M&N", "+q48uvdETJsT7c", WiFi
);


void setup() {
  // // put your setup code here, to run once:
  Serial.begin(9600);

  while(wifiMgr.manageConnection() != Comm::NetworkStatus::Connected) { }
  // WiFi.begin("M&N", "+q48uvdETJsT7c");
  // while(WiFi.status() != WL_CONNECTED);

#if WIFI_DEBUG
  Serial.println("Connected");
#endif

}

void loop() {
  // put your main code here, to run repeatedly:
}