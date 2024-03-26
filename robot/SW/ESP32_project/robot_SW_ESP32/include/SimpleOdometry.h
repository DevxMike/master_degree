#ifndef SimpleOdo_h
#define SimpleOdo_h

#include "interfaces/IOdometry.h"

namespace Sensor{

class simpleOdometry : public IOdometry{
public:
    simpleOdometry() noexcept;
    void poolOdometry(SensorManager& s) noexcept override;
};

}

#endif