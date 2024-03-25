#include "../include/OdometryManager.h"

namespace Sensor{

simpleOdometry::simpleOdometry() noexcept{
    resetPosition();
}

void simpleOdometry::poolOdometry(SensorManager& s) noexcept{
    // TBD 
}

void advancedOdometry::poolOdometry(SensorManager& s) noexcept{
    // TBD
}

OdometryManager::OdometryManager(
        custom::array<IOdometry*, availableAlgorithms>&& odo,
        SensorManager&& s
    ) noexcept:
    m_odometryAgents{ std::move(odo) }, m_sensorMgr{ std::move(s) },
    m_active{ ActiveOdometry::SimpleOdo } {}

std::size_t OdometryManager::getActiveOdometry() const noexcept{
    return m_active;
}

void OdometryManager::setActiveOdometry(ActiveOdometry o) noexcept{
    m_active = o;
    m_odometryAgents[m_active]->resetPosition();
}

void OdometryManager::updatePosition() noexcept{
    m_sensorMgr.poolSensors();
    // i dont know why, but when using operator[] i get memory dump
    (*(m_odometryAgents.begin() + (m_active % availableAlgorithms)))->poolOdometry(m_sensorMgr);
}

const position OdometryManager::getPosition() const noexcept{
    // i dont know why, but when using operator[] i get memory dump
    return (*(m_odometryAgents.begin() + (m_active % availableAlgorithms)))->getPosition();
}

}