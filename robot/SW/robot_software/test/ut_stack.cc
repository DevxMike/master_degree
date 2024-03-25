#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../include/stack.h"

TEST(StackTest, CanCreateEmptyStack) {
    using custom::stack;

    stack<int32_t, 10> s;

    EXPECT_EQ(s.size(), 10);
    EXPECT_EQ(s.count(), 0);
}

TEST(StackTest, CanPushAndPop) {
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

TEST(StackTest, CanCopyConstructAndCompare) {
    using custom::stack;

    stack<int32_t, 10> s;

    for(int i = 1; i <= 10; ++i){
        EXPECT_EQ(s.push(i), true);
    }

    EXPECT_EQ(s.full(), true);
    
    auto s1{ s };

    EXPECT_EQ(s == s1, true);

    while(!s1.empty()){
        auto tmp = std::move(s1.pop());
    }

    EXPECT_EQ(s1.empty(), true);
    EXPECT_EQ(s1 == s, false);
}

TEST(StackTest, CanMoveContruct) {
    using custom::stack;

    stack<int32_t, 10> s;

    for(int i = 1; i <= 10; ++i){
        EXPECT_EQ(s.push(i), true);
    }

    EXPECT_EQ(s.full(), true);
    
    auto s1{ std::move(s) };

    EXPECT_EQ(s == s1, false);
    EXPECT_EQ(s.empty(), true);
    EXPECT_EQ(s1.full(), true);
}

TEST(StackTest, CanCopyAssign) {
    using custom::stack;

    stack<int32_t, 10> s;

    for(int i = 1; i <= 10; ++i){
        EXPECT_EQ(s.push(i), true);
    }

    EXPECT_EQ(s.full(), true);
    
    stack<int32_t, 10> s1;

    s1 = s;

    EXPECT_EQ(s == s1, true);

    while(!s1.empty()){
        auto tmp = std::move(s1.pop());
    }

    EXPECT_EQ(s1.empty(), true);
    EXPECT_EQ(s1 == s, false);
}

TEST(StackTest, CanMoveAssign) {
    using custom::stack;

    stack<int32_t, 10> s;

    for(int i = 1; i <= 10; ++i){
        EXPECT_EQ(s.push(i), true);
    }

    EXPECT_EQ(s.full(), true);
    
    stack<int32_t, 10> s1;
    s1 = std::move(s);

    EXPECT_EQ(s == s1, false);
    EXPECT_EQ(s.empty(), true);
    EXPECT_EQ(s1.full(), true);
}