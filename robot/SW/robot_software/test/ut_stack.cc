#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../include/stack.h"

TEST(StackTest, CanCreateEmptyStack){
    using custom::stack;

    stack<int32_t, 10> s;

    EXPECT_EQ(s.size(), 10);
    EXPECT_EQ(s.count(), 0);
}

TEST(StackTest, CanPushAndPop){
    using custom::stack;

    stack<int32_t, 10> s;

    for(int i = 1; i <= 10; ++i){
        EXPECT_EQ(s.push(i), true);
    }

    EXPECT_EQ(s.full(), true);

    for(int i = 0; i < 10; ++i){
        auto tmp = std::move(s.pop());
        EXPECT_NE(tmp, 0);
    }

    EXPECT_EQ(s.count(), 0);
    EXPECT_EQ(s.empty(), true);
}

TEST(StackTest, CanCopyConstruct) {
 // TBD
}