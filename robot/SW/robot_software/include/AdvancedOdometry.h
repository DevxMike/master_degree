#pragma once

#include "SimpleOdometry.h"

namespace Sensor{

class advancedOdometry : public simpleOdometry{
public:
    using simpleOdometry::simpleOdometry;
    void poolOdometry(SensorManager& s) noexcept override;
};

}