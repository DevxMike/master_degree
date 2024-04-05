#ifndef wifiMGR_h
#define wifiMGR_h

#include "Arduino.h"
#include "WiFi.h"

namespace Comm{

enum class NetworkStatus{
    NotConnected,
    Connected
};

// using StringType = String;

template<class StringType>
class WiFiManager{
public:
    WiFiManager(const StringType& ssid, const StringType& password, WiFiClass& wifi_handle) noexcept: 
        m_ssid{ ssid }, m_password{ password }, m_wifi_handle{ wifi_handle }, m_internal_state{ Connecting } 
        {}

    WiFiManager(WiFiManager<StringType>&& w) noexcept:
        m_ssid{ std::move(w.m_ssid) }, m_password{ std::move(w.m_password) }, m_wifi_handle{ w.m_wifi_handle }, m_internal_state{ std::move(w.m_internal_state) }
        {}

    NetworkStatus manageConnection() noexcept{
        static unsigned long timer;
        
        switch(m_internal_state){
            case Connecting:
                // m_w.mode(WIFI_STA);
                m_wifi_handle.begin(m_ssid.c_str(), m_password.c_str());
                timer = millis();        
                m_internal_state = Timeout;
                return NetworkStatus::NotConnected;

                break;

            case Timeout:
                if((millis() - timer >= 500)){
                    m_internal_state = CheckingStatus;
                }
                return NetworkStatus::NotConnected;

                break;
            
            case CheckingStatus:
                if(m_wifi_handle.status() == WL_CONNECTED){
                    return NetworkStatus::Connected;
                }
                else{
                    m_internal_state = Connecting;
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
    uint8_t m_internal_state; 
    StringType m_ssid;
    StringType m_password;
    WiFiClass& m_wifi_handle;
};

}

#endif