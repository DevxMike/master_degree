#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../include/SensorManager.h"

class DummySensor : public Sensor::ISensor{
public:
    void init() override {
        State(Sensor::SensorStates::SensorInit);
    }

    ~DummySensor() override { }
    // DummySensor(){ }

    Sensor::SensorStates poolSensor() { 
        return Sensor::SensorStates::Timeout;
    }

    const void* getReadings() const{
        return nullptr;
    }
};

TEST(SensorTest, CanBuildInstance){
    std::array<Sensor::sensorPointer, Sensor::numSensors> sensors;

    for(auto& s : sensors){
       s = std::make_unique<DummySensor>();
    }

    Sensor::SensorManager mgr(std::move(sensors));

    for(uint8_t i = 0; i < Sensor::numSensors; ++i){
        auto index = static_cast<Sensor::SensorMapping>(i);

        EXPECT_NE(&mgr.getSensor(index), nullptr);
        EXPECT_EQ(mgr.getSensor(index).State(), Sensor::SensorStates::SensorInit);
    }
}