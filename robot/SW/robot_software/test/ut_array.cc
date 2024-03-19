#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>

#include "../include/array.h"
#include "../include/SensorManager.h"

TEST(CustomArrayTest, CanIterateOverArray){
    custom::array<int, 10> my_array;
    int i = 0;
    
    for(auto& x : my_array){
        my_array[i] = i;
        ++i;
    }

    i = 0;

    for(const auto& x : my_array){
        EXPECT_EQ(my_array[i], i);
        ++i;
    }
}

TEST(CustomArrayTest, CanMoveConstructArray1){
    custom::array<int, 10> my_array;
    int j = 0;
    
    for(auto& x : my_array){
        my_array[j] = j;
        ++j;
    }

    j = 0;

    custom::array<int, 10> dst = std::move(my_array);

    for(auto i = dst.begin(); i != dst.end(); ++i){
        EXPECT_EQ(*i, j);
        ++j;
    }
}

TEST(CustomArrayTest, CanMoveConstructArray2){
    custom::array<std::unique_ptr<int>, 10> my_array;
    custom::array<std::unique_ptr<int>, 10> dst = std::move(my_array);
}