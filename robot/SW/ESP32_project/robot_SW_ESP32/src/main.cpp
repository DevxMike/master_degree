#include <Arduino.h>
#include <array>
#include "WiFiManager.h"
#include "global_defines.h"
#include "array.h"
#include <initializer_list>
// put function declarations here:

// Comm::WiFiManager<String> wifiMgr(
//   "M&N", "+q48uvdETJsT7c"
// );

// custom::array<int, 10> a;
std::array<int, 10> a{{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }};

void setup() {
  // // put your setup code here, to run once:
  Serial.begin(9600);

  // while(wifiMgr.manageConnection() != Comm::NetworkStatus::Connected) { }
  WiFi.begin("M&N", "+q48uvdETJsT7c");
  while(WiFi.status() != WL_CONNECTED);

#if WIFI_DEBUG
  Serial.println("Connected");

  for(const auto& x: a){
    Serial.printf("%p -> %d\n", &x, x);
  }

#endif

}

void loop() {
  // put your main code here, to run repeatedly:
}