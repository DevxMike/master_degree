#ifndef Advanced_Odo_h
#define Advanced_Odo_h

#include "SimpleOdometry.h"

namespace Sensor{

class advancedOdometry : public simpleOdometry{
public:
    using simpleOdometry::simpleOdometry;
    void poolOdometry(SensorManager& s) noexcept override;
};

}

#endif