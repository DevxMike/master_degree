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
        m_SSID{ ssid }, m_password{ password } {}

    NetworkStatus manageConnection() noexcept{
        static uint8_t state = Connecting;

        switch(state){
            case Connecting:
                WiFi.mode(WIFI_STA);
                WiFi.begin(m_SSID, m_password);
                
        }
    }

private:
    enum : uint8_t{
        Connecting = 0,
        Timeout,
        Connected,
        CheckingStatus
    }

    StringType m_SSID;
    StringType m_password;
};

}

#endif