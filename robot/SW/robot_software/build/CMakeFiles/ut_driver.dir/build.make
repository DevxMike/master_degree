# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/build

# Include any dependencies generated for this target.
include CMakeFiles/ut_driver.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/ut_driver.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/ut_driver.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ut_driver.dir/flags.make

CMakeFiles/ut_driver.dir/test/ut_driver.cc.o: CMakeFiles/ut_driver.dir/flags.make
CMakeFiles/ut_driver.dir/test/ut_driver.cc.o: ../test/ut_driver.cc
CMakeFiles/ut_driver.dir/test/ut_driver.cc.o: CMakeFiles/ut_driver.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ut_driver.dir/test/ut_driver.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ut_driver.dir/test/ut_driver.cc.o -MF CMakeFiles/ut_driver.dir/test/ut_driver.cc.o.d -o CMakeFiles/ut_driver.dir/test/ut_driver.cc.o -c /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/test/ut_driver.cc

CMakeFiles/ut_driver.dir/test/ut_driver.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ut_driver.dir/test/ut_driver.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/test/ut_driver.cc > CMakeFiles/ut_driver.dir/test/ut_driver.cc.i

CMakeFiles/ut_driver.dir/test/ut_driver.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ut_driver.dir/test/ut_driver.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/test/ut_driver.cc -o CMakeFiles/ut_driver.dir/test/ut_driver.cc.s

CMakeFiles/ut_driver.dir/src/CommManager.cc.o: CMakeFiles/ut_driver.dir/flags.make
CMakeFiles/ut_driver.dir/src/CommManager.cc.o: ../src/CommManager.cc
CMakeFiles/ut_driver.dir/src/CommManager.cc.o: CMakeFiles/ut_driver.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/ut_driver.dir/src/CommManager.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ut_driver.dir/src/CommManager.cc.o -MF CMakeFiles/ut_driver.dir/src/CommManager.cc.o.d -o CMakeFiles/ut_driver.dir/src/CommManager.cc.o -c /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/src/CommManager.cc

CMakeFiles/ut_driver.dir/src/CommManager.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ut_driver.dir/src/CommManager.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/src/CommManager.cc > CMakeFiles/ut_driver.dir/src/CommManager.cc.i

CMakeFiles/ut_driver.dir/src/CommManager.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ut_driver.dir/src/CommManager.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/src/CommManager.cc -o CMakeFiles/ut_driver.dir/src/CommManager.cc.s

CMakeFiles/ut_driver.dir/src/DCMotor.cc.o: CMakeFiles/ut_driver.dir/flags.make
CMakeFiles/ut_driver.dir/src/DCMotor.cc.o: ../src/DCMotor.cc
CMakeFiles/ut_driver.dir/src/DCMotor.cc.o: CMakeFiles/ut_driver.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/ut_driver.dir/src/DCMotor.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ut_driver.dir/src/DCMotor.cc.o -MF CMakeFiles/ut_driver.dir/src/DCMotor.cc.o.d -o CMakeFiles/ut_driver.dir/src/DCMotor.cc.o -c /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/src/DCMotor.cc

CMakeFiles/ut_driver.dir/src/DCMotor.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ut_driver.dir/src/DCMotor.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/src/DCMotor.cc > CMakeFiles/ut_driver.dir/src/DCMotor.cc.i

CMakeFiles/ut_driver.dir/src/DCMotor.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ut_driver.dir/src/DCMotor.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/src/DCMotor.cc -o CMakeFiles/ut_driver.dir/src/DCMotor.cc.s

CMakeFiles/ut_driver.dir/src/DistanceSensor.cc.o: CMakeFiles/ut_driver.dir/flags.make
CMakeFiles/ut_driver.dir/src/DistanceSensor.cc.o: ../src/DistanceSensor.cc
CMakeFiles/ut_driver.dir/src/DistanceSensor.cc.o: CMakeFiles/ut_driver.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/ut_driver.dir/src/DistanceSensor.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ut_driver.dir/src/DistanceSensor.cc.o -MF CMakeFiles/ut_driver.dir/src/DistanceSensor.cc.o.d -o CMakeFiles/ut_driver.dir/src/DistanceSensor.cc.o -c /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/src/DistanceSensor.cc

CMakeFiles/ut_driver.dir/src/DistanceSensor.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ut_driver.dir/src/DistanceSensor.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/src/DistanceSensor.cc > CMakeFiles/ut_driver.dir/src/DistanceSensor.cc.i

CMakeFiles/ut_driver.dir/src/DistanceSensor.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ut_driver.dir/src/DistanceSensor.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/src/DistanceSensor.cc -o CMakeFiles/ut_driver.dir/src/DistanceSensor.cc.s

CMakeFiles/ut_driver.dir/src/Encoder.cc.o: CMakeFiles/ut_driver.dir/flags.make
CMakeFiles/ut_driver.dir/src/Encoder.cc.o: ../src/Encoder.cc
CMakeFiles/ut_driver.dir/src/Encoder.cc.o: CMakeFiles/ut_driver.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/ut_driver.dir/src/Encoder.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ut_driver.dir/src/Encoder.cc.o -MF CMakeFiles/ut_driver.dir/src/Encoder.cc.o.d -o CMakeFiles/ut_driver.dir/src/Encoder.cc.o -c /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/src/Encoder.cc

CMakeFiles/ut_driver.dir/src/Encoder.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ut_driver.dir/src/Encoder.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/src/Encoder.cc > CMakeFiles/ut_driver.dir/src/Encoder.cc.i

CMakeFiles/ut_driver.dir/src/Encoder.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ut_driver.dir/src/Encoder.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/src/Encoder.cc -o CMakeFiles/ut_driver.dir/src/Encoder.cc.s

CMakeFiles/ut_driver.dir/src/IMU.cc.o: CMakeFiles/ut_driver.dir/flags.make
CMakeFiles/ut_driver.dir/src/IMU.cc.o: ../src/IMU.cc
CMakeFiles/ut_driver.dir/src/IMU.cc.o: CMakeFiles/ut_driver.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/ut_driver.dir/src/IMU.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ut_driver.dir/src/IMU.cc.o -MF CMakeFiles/ut_driver.dir/src/IMU.cc.o.d -o CMakeFiles/ut_driver.dir/src/IMU.cc.o -c /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/src/IMU.cc

CMakeFiles/ut_driver.dir/src/IMU.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ut_driver.dir/src/IMU.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/src/IMU.cc > CMakeFiles/ut_driver.dir/src/IMU.cc.i

CMakeFiles/ut_driver.dir/src/IMU.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ut_driver.dir/src/IMU.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/src/IMU.cc -o CMakeFiles/ut_driver.dir/src/IMU.cc.s

CMakeFiles/ut_driver.dir/src/MotorManager.cc.o: CMakeFiles/ut_driver.dir/flags.make
CMakeFiles/ut_driver.dir/src/MotorManager.cc.o: ../src/MotorManager.cc
CMakeFiles/ut_driver.dir/src/MotorManager.cc.o: CMakeFiles/ut_driver.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/ut_driver.dir/src/MotorManager.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ut_driver.dir/src/MotorManager.cc.o -MF CMakeFiles/ut_driver.dir/src/MotorManager.cc.o.d -o CMakeFiles/ut_driver.dir/src/MotorManager.cc.o -c /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/src/MotorManager.cc

CMakeFiles/ut_driver.dir/src/MotorManager.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ut_driver.dir/src/MotorManager.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/src/MotorManager.cc > CMakeFiles/ut_driver.dir/src/MotorManager.cc.i

CMakeFiles/ut_driver.dir/src/MotorManager.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ut_driver.dir/src/MotorManager.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/src/MotorManager.cc -o CMakeFiles/ut_driver.dir/src/MotorManager.cc.s

CMakeFiles/ut_driver.dir/src/SensorManager.cc.o: CMakeFiles/ut_driver.dir/flags.make
CMakeFiles/ut_driver.dir/src/SensorManager.cc.o: ../src/SensorManager.cc
CMakeFiles/ut_driver.dir/src/SensorManager.cc.o: CMakeFiles/ut_driver.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/ut_driver.dir/src/SensorManager.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ut_driver.dir/src/SensorManager.cc.o -MF CMakeFiles/ut_driver.dir/src/SensorManager.cc.o.d -o CMakeFiles/ut_driver.dir/src/SensorManager.cc.o -c /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/src/SensorManager.cc

CMakeFiles/ut_driver.dir/src/SensorManager.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ut_driver.dir/src/SensorManager.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/src/SensorManager.cc > CMakeFiles/ut_driver.dir/src/SensorManager.cc.i

CMakeFiles/ut_driver.dir/src/SensorManager.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ut_driver.dir/src/SensorManager.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/src/SensorManager.cc -o CMakeFiles/ut_driver.dir/src/SensorManager.cc.s

CMakeFiles/ut_driver.dir/src/WiFiManager.cc.o: CMakeFiles/ut_driver.dir/flags.make
CMakeFiles/ut_driver.dir/src/WiFiManager.cc.o: ../src/WiFiManager.cc
CMakeFiles/ut_driver.dir/src/WiFiManager.cc.o: CMakeFiles/ut_driver.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/ut_driver.dir/src/WiFiManager.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ut_driver.dir/src/WiFiManager.cc.o -MF CMakeFiles/ut_driver.dir/src/WiFiManager.cc.o.d -o CMakeFiles/ut_driver.dir/src/WiFiManager.cc.o -c /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/src/WiFiManager.cc

CMakeFiles/ut_driver.dir/src/WiFiManager.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ut_driver.dir/src/WiFiManager.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/src/WiFiManager.cc > CMakeFiles/ut_driver.dir/src/WiFiManager.cc.i

CMakeFiles/ut_driver.dir/src/WiFiManager.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ut_driver.dir/src/WiFiManager.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/src/WiFiManager.cc -o CMakeFiles/ut_driver.dir/src/WiFiManager.cc.s

CMakeFiles/ut_driver.dir/src/driver.cc.o: CMakeFiles/ut_driver.dir/flags.make
CMakeFiles/ut_driver.dir/src/driver.cc.o: ../src/driver.cc
CMakeFiles/ut_driver.dir/src/driver.cc.o: CMakeFiles/ut_driver.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/ut_driver.dir/src/driver.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ut_driver.dir/src/driver.cc.o -MF CMakeFiles/ut_driver.dir/src/driver.cc.o.d -o CMakeFiles/ut_driver.dir/src/driver.cc.o -c /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/src/driver.cc

CMakeFiles/ut_driver.dir/src/driver.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ut_driver.dir/src/driver.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/src/driver.cc > CMakeFiles/ut_driver.dir/src/driver.cc.i

CMakeFiles/ut_driver.dir/src/driver.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ut_driver.dir/src/driver.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/src/driver.cc -o CMakeFiles/ut_driver.dir/src/driver.cc.s

# Object files for target ut_driver
ut_driver_OBJECTS = \
"CMakeFiles/ut_driver.dir/test/ut_driver.cc.o" \
"CMakeFiles/ut_driver.dir/src/CommManager.cc.o" \
"CMakeFiles/ut_driver.dir/src/DCMotor.cc.o" \
"CMakeFiles/ut_driver.dir/src/DistanceSensor.cc.o" \
"CMakeFiles/ut_driver.dir/src/Encoder.cc.o" \
"CMakeFiles/ut_driver.dir/src/IMU.cc.o" \
"CMakeFiles/ut_driver.dir/src/MotorManager.cc.o" \
"CMakeFiles/ut_driver.dir/src/SensorManager.cc.o" \
"CMakeFiles/ut_driver.dir/src/WiFiManager.cc.o" \
"CMakeFiles/ut_driver.dir/src/driver.cc.o"

# External object files for target ut_driver
ut_driver_EXTERNAL_OBJECTS =

ut_driver: CMakeFiles/ut_driver.dir/test/ut_driver.cc.o
ut_driver: CMakeFiles/ut_driver.dir/src/CommManager.cc.o
ut_driver: CMakeFiles/ut_driver.dir/src/DCMotor.cc.o
ut_driver: CMakeFiles/ut_driver.dir/src/DistanceSensor.cc.o
ut_driver: CMakeFiles/ut_driver.dir/src/Encoder.cc.o
ut_driver: CMakeFiles/ut_driver.dir/src/IMU.cc.o
ut_driver: CMakeFiles/ut_driver.dir/src/MotorManager.cc.o
ut_driver: CMakeFiles/ut_driver.dir/src/SensorManager.cc.o
ut_driver: CMakeFiles/ut_driver.dir/src/WiFiManager.cc.o
ut_driver: CMakeFiles/ut_driver.dir/src/driver.cc.o
ut_driver: CMakeFiles/ut_driver.dir/build.make
ut_driver: /usr/local/lib/libgtest.a
ut_driver: /usr/local/lib/libgtest_main.a
ut_driver: /usr/local/lib/libgtest.a
ut_driver: CMakeFiles/ut_driver.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Linking CXX executable ut_driver"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ut_driver.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ut_driver.dir/build: ut_driver
.PHONY : CMakeFiles/ut_driver.dir/build

CMakeFiles/ut_driver.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ut_driver.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ut_driver.dir/clean

CMakeFiles/ut_driver.dir/depend:
	cd /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/build /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/build /home/michael/Documents/dev/master/master_degree/robot/SW/robot_software/build/CMakeFiles/ut_driver.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ut_driver.dir/depend
