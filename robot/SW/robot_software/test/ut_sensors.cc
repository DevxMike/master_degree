#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../include/SensorManager.h"
#include "../include/DistanceSensor.h"
#include "../include/Encoder.h"
#include "../include/IMU.h"
#include "../include/AdvancedOdometry.h"

class DummySensor : public Sensor::ISensor{
public:
    void init() noexcept override {
        State(Sensor::SensorStates::SensorInit);
    }

    ~DummySensor() override { }
    // DummySensor(){ }

    Sensor::SensorStates poolSensor() noexcept { 
        return Sensor::SensorStates::Timeout;
    }

    const void* getReadings() const noexcept{
        return nullptr;
    }
};

TEST(SensorTest, CanBuildInstance){
    custom::array<Sensor::sensorPointer, Sensor::numSensors> sensors;

    for(auto& s : sensors){
       s = new DummySensor;
    }

    Sensor::SensorManager mgr(std::move(sensors));

    for(uint8_t i = 0; i < Sensor::numSensors; ++i){
        auto index = static_cast<Sensor::SensorMapping>(i);

        EXPECT_NE(&mgr.getSensor(index), nullptr);
        EXPECT_EQ(mgr.getSensor(index).State(), Sensor::SensorStates::SensorInit);
    }

    for(auto& s : sensors){
       delete s;
    }
}

TEST(SensorTest, CanReadDistanceSensor){
    using Sensor::DistanceSensor;
    using Sensor::test_value;

    DistanceSensor d(1, 2);

    auto reading = 
        reinterpret_cast<const DistanceSensor::reading_t*>(d.getReadings());

    EXPECT_EQ(test_value, *reading);
}

TEST(SensorTest, CanReadEncoder){
    using Sensor::Encoder;
    using Sensor::test_value_enc;

    Encoder e(1, 2);

    auto reading = 
        reinterpret_cast<const Encoder::reading_t*>(e.getReadings());

    EXPECT_EQ(test_value_enc, *reading);
}

TEST(SensorTest, CanResetEncoder){
    using Sensor::Encoder;
    using Sensor::test_value_enc;

    Encoder e(1, 2);

    auto reading = 
        reinterpret_cast<const Encoder::reading_t*>(e.getReadings());

    EXPECT_EQ(test_value_enc, *reading);

    e.reset();

    reading = 
        reinterpret_cast<const Encoder::reading_t*>(e.getReadings());

    EXPECT_EQ(0, *reading);
}

TEST(SensorTest, UseIntFalseByDefault){
    using Sensor::IMU;

    IMU imu(1,2);

    EXPECT_EQ(imu.useInterrupts(), false);
}

TEST(SensorTest, CanEnableInterruptUse){
    using Sensor::IMU;

    IMU imu(1,2,3);

    EXPECT_EQ(imu.useInterrupts(), true);
}

TEST(SensorTest, CanReadIMUAfterInit){
    using Sensor::IMU;

    IMU imu(1,2);

    imu.init();

    auto reading = 
        reinterpret_cast<const IMU::reading_t*>(imu.getReadings());

    float i {};

    for(const auto& x: (*reading)){
        EXPECT_EQ(i, x);
        i += 1.0f;
    }
}

bool operator==(const Sensor::position& s1, const Sensor::position& s2){
    return true;
}

TEST(OdometryTest, CanCreateInstanceOfSimpleOdometry){
    Sensor::simpleOdometry odo;

    auto result = odo.getPosition();
    auto expected = Sensor::position{ .0f, .0f, .0f };

    EXPECT_EQ(result == expected, true);
}

TEST(OdometryTest, CanCreateInstanceOfAdvancedOdometry){
    Sensor::advancedOdometry odo;

    auto result = odo.getPosition();
    auto expected = Sensor::position{ .0f, .0f, .0f };

    EXPECT_EQ(result == expected, true);
}

TEST(OdometryTest, CanCreateArrayOfIodometryObjects){
    Sensor::simpleOdometry s_odo;
    Sensor::advancedOdometry a_odo;

    custom::array<Sensor::IOdometry*, 2> a;

    a[0] = &s_odo;
    a[1] = &a_odo;

    for(const auto& o: a){
        auto result = o->getPosition();
        auto expected = Sensor::position{ .0f, .0f, .0f };

        EXPECT_EQ(result == expected, true);
    }
}

TEST(OdometryTest, CanSetAndResetPosition){
    Sensor::simpleOdometry s_odo;
    Sensor::advancedOdometry a_odo;

    custom::array<Sensor::IOdometry*, 2> a;

    a[0] = &s_odo;
    a[1] = &a_odo;

    for(const auto& o: a){
        auto expected_reset = Sensor::position{ .0f, .0f, .0f };
        auto expected_set = Sensor::position{ .1f, .2f, .3f };

        auto result = o->getPosition();
        EXPECT_EQ(result == expected_reset, true);

        o->setPosition(expected_set);

        result = o->getPosition();
        EXPECT_EQ(result == expected_set, true);

        o->resetPosition();

        result = o->getPosition();
        EXPECT_EQ(result == expected_reset, true);
    }
}