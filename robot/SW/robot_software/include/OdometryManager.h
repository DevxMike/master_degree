#pragma once

#include "../include/AdvancedOdometry.h"
#include "../include/array.h"

namespace Sensor{

enum ActiveOdometry : std::size_t {
    SimpleOdo = 0,
    AdvancedOdo = 1
};

constexpr std::size_t availableAlgorithms = 2;

class OdometryManager{
public:
    OdometryManager(
        custom::array<IOdometry*, availableAlgorithms>&& odo,
        SensorManager&& s
    ) noexcept;

    void updatePosition() noexcept;
    void setActiveOdometry(ActiveOdometry o) noexcept;
    std::size_t getActiveOdometry() const noexcept;
    const position getPosition() const noexcept;
private:
    custom::array<IOdometry*, availableAlgorithms> m_odometryAgents;
    SensorManager m_sensorMgr;
    std::size_t m_active;
};

}