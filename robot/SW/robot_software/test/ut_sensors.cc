#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../include/SensorManager.h"
#include "../include/DistanceSensor.h"
#include "../include/Encoder.h"
#include "../include/IMU.h"
#include "../include/OdometryManager.h"

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

TEST(OdometryTest, CanCreateInstanceOfOdometryManager){
    using namespace Sensor;

    custom::array<Sensor::IOdometry*, 2> algorithms;
    simpleOdometry s_odo;
    advancedOdometry a_odo;
    algorithms[ActiveOdometry::SimpleOdo] = &s_odo;
    algorithms[ActiveOdometry::AdvancedOdo] = &a_odo;


    custom::array<ISensor*, numSensors> sensors;
    Encoder enc1(1, 2), enc2(1, 2);
    IMU imu(1, 2);
    DistanceSensor d1(1, 2), d2(1, 2), d3(1, 3), d4(1, 2);


    sensors[translate(SensorMapping::LEFT_Encoder)] = &enc1;
    sensors[translate(SensorMapping::RIGHT_Encoder)] = &enc2;
    sensors[translate(SensorMapping::DST_Front_1)] = &d1;
    sensors[translate(SensorMapping::DST_Front_2)] = &d2;
    sensors[translate(SensorMapping::DST_Front_3)] = &d3;
    sensors[translate(SensorMapping::DST_Rear)] = &d4;
    sensors[translate(SensorMapping::IMU)] = &imu;

    SensorManager s_mgr(std::move(sensors));

    OdometryManager o_mgr(
        std::move(algorithms),
        std::move(s_mgr)
    );

    auto result = o_mgr.getActiveOdometry();

    EXPECT_EQ(result, ActiveOdometry::SimpleOdo);
}

TEST(OdometryTest, CanSwitchOdometry){
    using namespace Sensor;

    custom::array<Sensor::IOdometry*, 2> algorithms;
    simpleOdometry s_odo;
    advancedOdometry a_odo;
    algorithms[ActiveOdometry::SimpleOdo] = &s_odo;
    algorithms[ActiveOdometry::AdvancedOdo] = &a_odo;


    custom::array<ISensor*, numSensors> sensors;
    Encoder enc1(1, 2), enc2(1, 2);
    IMU imu(1, 2);
    DistanceSensor d1(1, 2), d2(1, 2), d3(1, 3), d4(1, 2);


    sensors[translate(SensorMapping::LEFT_Encoder)] = &enc1;
    sensors[translate(SensorMapping::RIGHT_Encoder)] = &enc2;
    sensors[translate(SensorMapping::DST_Front_1)] = &d1;
    sensors[translate(SensorMapping::DST_Front_2)] = &d2;
    sensors[translate(SensorMapping::DST_Front_3)] = &d3;
    sensors[translate(SensorMapping::DST_Rear)] = &d4;
    sensors[translate(SensorMapping::IMU)] = &imu;

    SensorManager s_mgr(std::move(sensors));

    OdometryManager o_mgr(
        std::move(algorithms),
        std::move(s_mgr)
    );

    auto result = o_mgr.getActiveOdometry();

    EXPECT_EQ(result, ActiveOdometry::SimpleOdo);

    o_mgr.setActiveOdometry(ActiveOdometry::AdvancedOdo);

    result = o_mgr.getActiveOdometry();

    EXPECT_EQ(result, ActiveOdometry::AdvancedOdo);

    const auto& pos = o_mgr.getPosition();
    auto expected = position{ 0, 0, 0 };

    EXPECT_EQ(pos == expected, true);
}

TEST(OdometryTest, CanPoolOdometry){
    using namespace Sensor;

    custom::array<Sensor::IOdometry*, 2> algorithms;
    simpleOdometry s_odo;
    advancedOdometry a_odo;
    algorithms[ActiveOdometry::SimpleOdo] = &s_odo;
    algorithms[ActiveOdometry::AdvancedOdo] = &a_odo;


    custom::array<ISensor*, numSensors> sensors;
    Encoder enc1(1, 2), enc2(1, 2);
    IMU imu(1, 2);
    DistanceSensor d1(1, 2), d2(1, 2), d3(1, 3), d4(1, 2);


    sensors[translate(SensorMapping::LEFT_Encoder)] = &enc1;
    sensors[translate(SensorMapping::RIGHT_Encoder)] = &enc2;
    sensors[translate(SensorMapping::DST_Front_1)] = &d1;
    sensors[translate(SensorMapping::DST_Front_2)] = &d2;
    sensors[translate(SensorMapping::DST_Front_3)] = &d3;
    sensors[translate(SensorMapping::DST_Rear)] = &d4;
    sensors[translate(SensorMapping::IMU)] = &imu;

    SensorManager s_mgr(std::move(sensors));

    OdometryManager o_mgr(
        std::move(algorithms),
        std::move(s_mgr)
    );

    auto result = o_mgr.getActiveOdometry();

    EXPECT_EQ(result, ActiveOdometry::SimpleOdo);

    o_mgr.setActiveOdometry(ActiveOdometry::AdvancedOdo);

    result = o_mgr.getActiveOdometry();

    EXPECT_EQ(result, ActiveOdometry::AdvancedOdo);

    const auto& pos = o_mgr.getPosition();
    auto expected = position{ 0, 0, 0 };

    EXPECT_EQ(pos == expected, true);

    o_mgr.updatePosition();

    o_mgr.setActiveOdometry(ActiveOdometry::SimpleOdo);

    result = o_mgr.getActiveOdometry();
    EXPECT_EQ(result, ActiveOdometry::SimpleOdo);
    
    o_mgr.updatePosition();
}