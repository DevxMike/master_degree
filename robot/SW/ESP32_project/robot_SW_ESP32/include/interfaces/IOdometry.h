#ifndef IOdometry_h
#define IOdometry_h

#include "../SensorManager.h"

namespace Sensor{

struct position{
        float x;
        float y;
        float theta;
};

class IOdometry{
public:
    virtual ~IOdometry() { }
    virtual void poolOdometry(SensorManager& s) noexcept = 0;
    

    virtual void setPosition(const position& p) noexcept final {
        m_pos = p;
    }
    virtual void resetPosition() noexcept final {
        m_pos.theta = m_pos.x = m_pos.y = 0.0f;
    }
    virtual const position& getPosition() const noexcept final {
        return m_pos;
    }
    

private:
    position m_pos;
};

}

#endif