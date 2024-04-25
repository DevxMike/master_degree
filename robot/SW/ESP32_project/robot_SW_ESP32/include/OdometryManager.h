#ifndef OdoMgr_h
#define OdoMgr_h

#include "../include/AdvancedOdometry.h"
#include <array>

namespace Sensor{

enum ActiveOdometry : std::size_t {
    SimpleOdo = 0,
    AdvancedOdo = 1
};

constexpr std::size_t availableAlgorithms = 2;

class OdometryManager{
public:
    OdometryManager(
        std::array<IOdometry*, availableAlgorithms>&& odo,
        SensorManager& s
    ) noexcept;

    void updatePosition() noexcept;
    void setActiveOdometry(ActiveOdometry o) noexcept;
    std::size_t getActiveOdometry() const noexcept;
    const position getPosition() const noexcept;
    void resetActiveOdometry() noexcept;
private:
    std::array<IOdometry*, availableAlgorithms> m_odometryAgents;
    SensorManager& m_sensorMgr;
    std::size_t m_active;
};

}

#endif