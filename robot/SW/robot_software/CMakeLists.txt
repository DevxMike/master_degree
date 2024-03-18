cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Locate Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Include directories
include_directories(include)

# Add source files
file(GLOB SOURCES "src/*.cc")

# Build executable for tests
add_executable(ut_comm test/ut_comm.cc ${SOURCES})
add_executable(ut_driver test/ut_driver.cc ${SOURCES})
add_executable(ut_motors test/ut_motors.cc ${SOURCES})
add_executable(ut_sensors test/ut_sensors.cc ${SOURCES})

# Link Google Test libraries
target_link_libraries(ut_comm ${GTEST_BOTH_LIBRARIES} pthread)
target_link_libraries(ut_driver ${GTEST_BOTH_LIBRARIES} pthread)
target_link_libraries(ut_motors ${GTEST_BOTH_LIBRARIES} pthread)
target_link_libraries(ut_sensors ${GTEST_BOTH_LIBRARIES} pthread)