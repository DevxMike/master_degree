#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <vector>

#include "../include/DCMotor.h"

uint32_t dummyMapper(uint32_t v){
    return v*10;
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