#ifndef CommMgr_h
#define CommMgr_h

#include <functional>

#include "PubSubClient.h"
#include "WiFiManager.h"
#include "stack.h"

namespace Comm{
namespace MQTT{

constexpr std::size_t stack_depth = 20;
constexpr unsigned MQTT_port = 1883;

enum class MQTTStatus{
    NotConnected,
    BrokerNotConnected,
    NetworkConnected,
    BrokerConnected
};

template<class StringType>
class CommManager{
public:
    using messageType = std::tuple<StringType, StringType>;
    
    static messageType createMessage(const StringType& topic, const StringType& payload) noexcept {
        return messageType(topic, payload);
    }

    CommManager(WiFiManager<StringType>&& w, const StringType& uname, const StringType& broker, const StringType& password, WiFiClient& c, const StringType& ID = "Robot") noexcept:
        m_wifiMgr{ std::move(w) }, 
        m_messageStack{  }, 
        m_broker{ broker },
        m_uname{ uname },
        m_password{ password },
        m_ID{ ID },
        m_internalState { ConnectingNetwork },
        m_mqttClient{ c } {}

    MQTTStatus poolCommManager() noexcept {
        static unsigned long timer;
        static unsigned long timeout;
        static uint8_t previous_state;

        switch(m_internalState){
            case ConnectingNetwork:
                if(poolNetwork() == Comm::NetworkStatus::Connected){
                    m_internalState = ConnectingBroker;

                    return MQTTStatus::NetworkConnected;
                }
                else{
                    previous_state = m_internalState;
                    m_internalState = Timeout;
                    timeout = 600;
                    timer = millis();

                    return MQTTStatus::NotConnected;
                }
            break;

            case Timeout:
                if(millis() - timer > timeout){
                    m_internalState = previous_state;
                }

                return MQTTStatus::NotConnected;
            break;

            case ConnectingBroker:
                m_mqttClient.setServer(m_broker.c_str(), MQTT_port);
                m_mqttClient.connect(m_ID.c_str(), m_uname.c_str(), m_password.c_str());

                if(m_mqttClient.connected()){
                    m_internalState = CheckingStatus;
                    
                    // TODO
                    // register callback
                    // subscribe

                    return MQTTStatus::BrokerConnected;
                }
                else{
                    previous_state = m_internalState;
                    m_internalState = Timeout;
                    timeout = 300;
                    timer = millis();

                    return MQTTStatus::BrokerNotConnected;
                }
            break;

            case CheckingStatus:
                if(poolNetwork() != NetworkStatus::Connected){
                    m_internalState = ConnectingNetwork;

                    return MQTTStatus::NotConnected;
                }
                else if(!m_mqttClient.connected()){
                    m_internalState = ConnectingBroker;

                    return MQTTStatus::BrokerNotConnected;
                }
                
                m_mqttClient.loop();

                while(!m_messageStack.empty()){
                    auto message = m_messageStack.pop();
                    auto topic = std::get<0>(message);
                    auto payload = std::get<1>(message);

                    m_mqttClient.publish(topic.c_str(), payload.c_str());
                }

                return MQTTStatus::BrokerConnected;
            break;
        }

        return MQTTStatus::NotConnected;
    }
    
    bool sendMessage(messageType&& message) noexcept {
        return m_messageStack.push(message);
    }

private:
    NetworkStatus poolNetwork() noexcept {
        return m_wifiMgr.manageConnection();
    }

    enum : uint8_t{
        ConnectingNetwork = 0,
        ConnectingBroker,
        Timeout,
        CheckingStatus
    };
    uint8_t m_internalState;
    WiFiManager<StringType> m_wifiMgr; // network management
    custom::stack<messageType, stack_depth> m_messageStack;
    StringType m_broker; // MQTT access
    StringType m_password;
    StringType m_uname;
    StringType m_ID;
    PubSubClient m_mqttClient;
};

} // MQTT
} // Comm

#endif