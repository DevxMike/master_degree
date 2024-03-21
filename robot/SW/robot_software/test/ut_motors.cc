#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <vector>

#include "../include/MotorManager.h"

uint32_t dummyMapper(uint32_t v){
    return v;
}

TEST(MotorTest, RotateLeftTest){
    using Motor::motorSignals;
    using Motor::MotorMapper;
    using Motor::max_absolute_v;
    using Motor::epsilon_abs;

    motorSignals s;

    std::vector<int32_t> v_in{
        0, 1, 2, 9, 10, 11, 20, 50, max_absolute_v + 10
    };

    std::vector<uint32_t> v_out;

    for(const auto& x: v_in){
        uint32_t v;

        if(x > epsilon_abs){
            auto tmp = (x > max_absolute_v)? max_absolute_v : x;
            v = dummyMapper(static_cast<uint32_t>(tmp));
        }
        else{
            v = 0;
        }
        v_out.push_back(v);
    }

    ASSERT_EQ(v_in.size(), v_out.size());

    for(std::size_t i = 0; i < v_in.size(); ++i){
        MotorMapper(v_in[i], s, dummyMapper);
        EXPECT_EQ(s.pwm, v_out[i]);
        EXPECT_EQ(s.dir, false);
    }

}

TEST(MotorTest, RotateRightTest){
    using Motor::motorSignals;
    using Motor::MotorMapper;
    using Motor::max_absolute_v;
    using Motor::epsilon_abs;

    motorSignals s;

    std::vector<int32_t> v_in{
        0, -1, -2, -9, -10, -11, -20, -50, -max_absolute_v - 10
    };

    std::vector<uint32_t> v_out;
    std::vector<bool> dirs;

    for(const auto& x: v_in){
        uint32_t v;
        bool dir;

        if(x < -epsilon_abs){
            auto tmp = (x < -max_absolute_v)? max_absolute_v : -x;
            v = dummyMapper(static_cast<uint32_t>(tmp));
            dir = true;
        }
        else{
            dir = false;
            v = 0;
        }
        v_out.push_back(v);
        dirs.push_back(dir);
    }

    ASSERT_EQ(v_in.size(), v_out.size());
    ASSERT_EQ(v_in.size(), dirs.size());

    for(std::size_t i = 0; i < v_in.size(); ++i){
        MotorMapper(v_in[i], s, dummyMapper);
        EXPECT_EQ(s.pwm, v_out[i]);
        EXPECT_EQ(s.dir, dirs[i]);
        EXPECT_EQ(
            s.pwm >= 0, true
        );
    }

}

auto cmp = [](const auto& m, auto v_ex, bool dir_ex){
    EXPECT_EQ(m.dir, dir_ex);
    EXPECT_EQ(m.pwm, v_ex);
};

const auto outputs = Motor::DCMotor::outputs{
    1, 2, 3
};

TEST(MotorTest, CanCreateMotorInstance){
    using Motor::DCMotor;

    DCMotor m{ dummyMapper, outputs };

    auto tmp = m.getCurrentSpeed();

    EXPECT_EQ(tmp.dir, false);
    EXPECT_EQ(tmp.pwm, 0);
}



TEST(MotorTest, CanSetMotorSpeed1){
    using Motor::DCMotor;

    DCMotor m{ dummyMapper, outputs };
    const int32_t v_in = Motor::epsilon_abs;
    const uint32_t expected_speed = 0;
    const bool expected_dir = false;

    m.setSpeed(v_in);
    cmp(
        m.getCurrentSpeed(), expected_speed, expected_dir
    );
}

TEST(MotorTest, CanSetMotorSpeed2){
    using Motor::DCMotor;

    DCMotor m{ dummyMapper, outputs };
    const int32_t v_in = Motor::epsilon_abs + 1;
    const uint32_t expected_speed = Motor::epsilon_abs + 1;
    const bool expected_dir = false;

    m.setSpeed(v_in);
    cmp(
        m.getCurrentSpeed(), expected_speed, expected_dir
    );
}

TEST(MotorTest, CanSetMotorSpeed3){
    using Motor::DCMotor;

    DCMotor m{ dummyMapper, outputs };
    const int32_t v_in = Motor::max_absolute_v;
    const uint32_t expected_speed = Motor::max_absolute_v;
    const bool expected_dir = false;

    m.setSpeed(v_in);
    cmp(
        m.getCurrentSpeed(), expected_speed, expected_dir
    );
}

TEST(MotorTest, CanSetMotorSpeed4){
    using Motor::DCMotor;

    DCMotor m{ dummyMapper, outputs };
    const int32_t v_in = Motor::max_absolute_v + 1;
    const uint32_t expected_speed = Motor::max_absolute_v;
    const bool expected_dir = false;

    m.setSpeed(v_in);
    cmp(
        m.getCurrentSpeed(), expected_speed, expected_dir
    );
}

TEST(MotorTest, CanSetMotorSpeed5){
    using Motor::DCMotor;

    DCMotor m{ dummyMapper, outputs };
    const int32_t v_in = -Motor::epsilon_abs;
    const uint32_t expected_speed = 0;
    const bool expected_dir = false;

    m.setSpeed(v_in);
    cmp(
        m.getCurrentSpeed(), expected_speed, expected_dir
    );
}

TEST(MotorTest, CanSetMotorSpeed6){
    using Motor::DCMotor;

    DCMotor m{ dummyMapper, outputs };
    const int32_t v_in = -Motor::epsilon_abs - 1;
    const uint32_t expected_speed = Motor::epsilon_abs + 1;
    const bool expected_dir = true;

    m.setSpeed(v_in);
    cmp(
        m.getCurrentSpeed(), expected_speed, expected_dir
    );
}

TEST(MotorTest, CanSetMotorSpeed7){
    using Motor::DCMotor;

    DCMotor m{ dummyMapper, outputs };
    const int32_t v_in = -Motor::max_absolute_v;
    const uint32_t expected_speed = Motor::max_absolute_v;
    const bool expected_dir = true;

    m.setSpeed(v_in);
    cmp(
        m.getCurrentSpeed(), expected_speed, expected_dir
    );
}

TEST(MotorTest, CanSetMotorSpeed8){
    using Motor::DCMotor;

    DCMotor m{ dummyMapper, outputs };
    const int32_t v_in = -Motor::max_absolute_v - 1;
    const uint32_t expected_speed = Motor::max_absolute_v;
    const bool expected_dir = true;

    m.setSpeed(v_in);
    cmp(
        m.getCurrentSpeed(), expected_speed, expected_dir
    );
}

TEST(MotorTest, CanCreateMotorManager){
    using namespace Motor;

    DCMotor left{ dummyMapper, outputs };
    DCMotor right{ dummyMapper, outputs };
    MotorManager::motor_array motors;
    
    motors[0] = &left;
    motors[1] = &right;

    MotorManager mgr{ std::move(motors) };
    mgr.init();

    EXPECT_EQ(mgr.InertiaCoef(), inertia_coef);
}

TEST(MotorTest, CanSetSpeedWithMotorManager){
    using namespace Motor;

    DCMotor left{ dummyMapper, outputs };
    DCMotor right{ dummyMapper, outputs };
    MotorManager::motor_array motors;
    
    motors[0] = &left;
    motors[1] = &right;

    MotorManager mgr{ std::move(motors) };

    MotorManager::speed_array desired; 
    desired[0] = 40;
    desired[1] = -30;

    mgr.init();

    mgr.setSpeed(desired);
    mgr.poolMotors();

    auto d_mgr = mgr.DesiredSpeed();
    auto c_mgr = mgr.CurrentSpeed();

    auto d = d_mgr->begin();
    auto c = c_mgr->begin();
    auto _d = desired.begin();

    for(;
        d != d_mgr->end() && c != c_mgr->end() && _d != desired.end();
        ++d, ++c, ++_d
    ){
        EXPECT_EQ(*d, *_d);
        EXPECT_NE(*d, *c);
    }
}
