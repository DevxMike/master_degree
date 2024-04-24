#ifndef driver_h
#define driver_h

#include <Arduino.h>
#include <array>
#include "CommManager.h"
#include "global_defines.h"
#include "array.h"
#include <initializer_list>
#include "stack.h"
#include "DistanceSensor.h"
#include "MotorManager.h"
#include "ArduinoJson.h"
#include "Encoder.h"
#include "global_defines.h"
#include "SensorManager.h"

class Kernel{
public:
    static void init();
    static void main();
    
    /* motion management */
    static Motor::DCMotor motorLeft;
    static Motor::DCMotor motorRight;
    static Motor::MotorManager motorManager;
    static uint32_t mapper(uint32_t);
    static constants::motors::types::motor_pid pidLeft;
    static constants::motors::types::motor_pid pidRight;

    /* communications management */
    static constants::comm::types::job_stack_t jobStack;
    static Comm::MQTT::CommManager<String, constants::comm::subscribedTopics> commMgr;
    static void MQTTcallback(char*, byte*, unsigned int);

    /* sensor management */
    static Sensor::DistanceSensor rear;
    static Sensor::DistanceSensor front_left;
    static Sensor::DistanceSensor front;
    static Sensor::DistanceSensor front_right;

    static Sensor::Encoder encoderLeft;
    static Sensor::Encoder encoderRight;
    
    static Sensor::SensorManager sensorMgr;
    // TODO

    /*  */
};


#endif