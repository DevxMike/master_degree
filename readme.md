# Usage Notice for this Repository

Dear Users,

This repository is intended for non-commercial, private use only. Individuals are allowed to edit or fork the project under the condition of providing proper attribution to both the repository and its authors. It is crucial to quote the repository and acknowledge the authors appropriately. Please note that the authors of this solution do not carry any responsibility for any consequences arising from the use of this project.

# Master's Thesis: Navigation Algorithm Study

This repository contains the code and documentation for a master's thesis focused on the investigation of navigation algorithms. The thesis encompasses the following areas:

    Robot Construction:
        Design and construction of the robot platform for experimental purposes.

    ROS2 System Development:
        Development of a Robot Operating System 2 (ROS2) based system for controlling the robot.

    Navigation Algorithm Study:
        Exploration and analysis of various navigation algorithms.
        Examination of different odometry algorithms for localization and movement estimation.

# Repository Structure

    robot/: Contains documentation, schematics, and code related to the construction of the robot platform.
    driver/: Includes ROS2 packages developed for controlling the robot.
    research/: Contains docs related to experiments.
    thesis/: Contains my thesis.

Each directory contains specific instructions and documentation regarding its usage and purpose.  

# Main documentation files

    robot/HW - robot/HW/HW_desc
    robot/SW - robot/SW/SW_desc
    ROS2 system - driver/DRIVER_desc
    research - research/research_desc

# Thesis location

    thesis/weiszablon.pdf

# Dependencies

    robot: to run unit tests you will need
        - cmake 3.10 or higher
        - GTest library
    arduino software (required libraries):
        - ArduinoJson
        - String (Arduino)
        - PubSubClient    
        - internal arduino libs for use of digital I/O, ext INT or PWM 

    ROS2 system: 
	TBD
