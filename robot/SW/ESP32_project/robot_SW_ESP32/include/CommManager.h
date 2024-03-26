#ifndef CommMgr_h
#define CommMgr_h

#include "WiFiManager.h"
#include "stack.h"

namespace Comm{
namespace MQTT{

constexpr std::size_t stack_depth = 20;

enum class MQTTStatus{
    NotConnected,
    Connected
};

template<class StringType>
class CommManager{
public:
    CommManager(
        WifiManager&& w, 
        const StringType& device_id, 
        const StringType& broker, 
        StringType password
    ) noexcept:
        m_wifiMgr{ std::move(w) }, 
        m_messageStack{  }, 
        m_broker{ broker },
        m_deviceID{ device_id },
        m_password{ password } {  }

    MQTTStatus poolCommManager() noexcept {
        // TBD
        return MQTTStatus::NotConnected;
    }
    
    bool sendMessage(const StringType& message) noexcept {
        return m_messageStack.push(message);
    }

private:
    NetworkStatus poolNetwork() noexcept {
        // TBD
        return NetworkStatus::NotConnected;
    }

    WiFiManager m_wifiMgr; // network management
    custom::stack<StringType, stack_depth> m_messageStack;
    StringType m_broker; // MQTT access
    StringType m_password;
    StringType m_deviceID;
};

} // MQTT
} // Comm

#endif