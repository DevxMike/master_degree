#include "../include/OdometryManager.h"
#include "../include/Encoder.h"
#include "../include/global_defines.h"

namespace Sensor{

constexpr float getAng(int32_t numImpulses){
      return 
        (1.0f/constants::experiment::samplingTime)
        * (static_cast<float>(numImpulses)/constants::motors::impulsesPerRotation)
        * 2
        * PI;
    }

simpleOdometry::simpleOdometry() noexcept{
    resetPosition();
}

void simpleOdometry::poolOdometry(SensorManager& s) noexcept{

    Sensor::Encoder* leftEnc =  reinterpret_cast<Sensor::Encoder*>(&s.getSensor(SensorMapping::LEFT_Encoder));
    Sensor::Encoder* rightEnc = reinterpret_cast<Sensor::Encoder*>(&s.getSensor(SensorMapping::RIGHT_Encoder));

    auto& pos = getPosition();

    const int32_t* reading1 = Sensor::get_reading<Sensor::Encoder::reading_t>(*leftEnc);
    const int32_t* reading2 = Sensor::get_reading<Sensor::Encoder::reading_t>(*rightEnc);
    
    float angLeft = getAng(*reading1);
    float angRight = getAng(*reading2);

    position new_pos = pos;

    new_pos.angularLeft = new_pos.angularLeft * 0.7 + 0.3 * angLeft;
    new_pos.angularRight = new_pos.angularRight * 0.7 + 0.3 * angRight;

    leftEnc->reset();
    rightEnc->reset();

    #if SENSOR_DEBUG
        Serial.println(new_pos.angularRight);
    #endif

    setPosition(new_pos);
}

void advancedOdometry::poolOdometry(SensorManager& s) noexcept{
    // wont be implemented in upcoming release
}

OdometryManager::OdometryManager(
        std::array<IOdometry*, availableAlgorithms>&& odo,
        SensorManager& s
    ) noexcept:
    m_odometryAgents{ std::move(odo) }, m_sensorMgr{ s },
    m_active{ ActiveOdometry::SimpleOdo } {}

std::size_t OdometryManager::getActiveOdometry() const noexcept{
    return m_active;
}

void OdometryManager::setActiveOdometry(ActiveOdometry o) noexcept{
    m_active = o;
    m_odometryAgents[m_active]->resetPosition();
}

void OdometryManager::updatePosition() noexcept{
    m_odometryAgents[m_active]->poolOdometry(m_sensorMgr);
}

const position OdometryManager::getPosition() const noexcept{
    return m_odometryAgents[m_active]->getPosition();
}

}
