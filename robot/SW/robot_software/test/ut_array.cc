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

TEST(CustomArrayTest, CanCompareArrays1){
    custom::array<int32_t, 10> i;
    custom::array<int32_t, 10> j;

    for(std::size_t k = 0; k < 10; ++k){
        i[k] = j[k] = k * 2 + 10;
    }

    EXPECT_EQ(i == j, true);
}

TEST(CustomArrayTest, CanCompareArrays2){
    custom::array<int32_t, 10> i;
    custom::array<int32_t, 10> j;

    for(std::size_t k = 0; k < 10; ++k){
        i[k] = k * 2 + 11;
        j[k] = k * 2 + 10;
    }

    EXPECT_EQ(i == j, false);
}

TEST(CustomArrayTest, CanCopyArray){
    custom::array<int32_t, 10> i;
    custom::array<int32_t, 10> j;

    for(std::size_t k = 0; k < 10; ++k){
        i[k] = k * 2 + 10;
    }

    j = i;

    EXPECT_EQ(i == j, true);
}

TEST(CustomArrayTest, CanMoveArray){
    custom::array<int32_t, 10> i;
    custom::array<int32_t, 10> j;

    for(std::size_t k = 0; k < 10; ++k){
        i[k] = k * 2 + 10;
    }

    j = std::move(i);
}

TEST(CustomArrayTest, CanCopyConstruct){
    custom::array<int32_t, 10> i;
    

    for(std::size_t k = 0; k < 10; ++k){
        i[k] = k * 2 + 10;
    }

    custom::array<int32_t, 10> j{ i };

    EXPECT_EQ(i == j, true);
}

TEST(CustomArrayTest, CanTakeReference){
    custom::array<int32_t, 10> i;
    

    for(std::size_t k = 0; k < 10; ++k){
        i[k] = k * 2 + 10;
    }

    custom::array<int32_t, 10>& j{ i };

    EXPECT_EQ(i == j, true);
    EXPECT_EQ(&i, &j);
}