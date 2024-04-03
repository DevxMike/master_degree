#ifndef wifiMGR_h
#define wifiMGR_h

#include "Arduino.h"
#include "WiFi.h"

namespace Comm{

enum class NetworkStatus{
    NotConnected,
    Connected
};

template<class StringType>
class WiFiManager{
public:
    WiFiManager(const StringType& ssid, const StringType& password) noexcept: 
        m_ssid{ ssid }, m_password{ password } {}

    NetworkStatus manageConnection() noexcept{
        static uint8_t state = Connecting;
        static unsigned long timer;
        
        switch(state){
            case Connecting:
                // m_w.mode(WIFI_STA);
                WiFi.begin(m_ssid.c_str(), m_password.c_str());
                timer = millis();        
                state = Timeout;
                return NetworkStatus::NotConnected;

                break;

            case Timeout:
                if((millis() - timer >= 500)){
                    state = CheckingStatus;
                }
                return NetworkStatus::NotConnected;

                break;
            
            case CheckingStatus:
                if(WiFi.status() == WL_CONNECTED){
                    return NetworkStatus::Connected;
                }
                else{
                    state = Connecting;
                    return NetworkStatus::NotConnected;
                }
                break;
        }

        return NetworkStatus::NotConnected;
    }

private:
    enum : uint8_t{
        Connecting = 0,
        Timeout,
        CheckingStatus
    };

    StringType m_ssid;
    StringType m_password;
};

}

#endif