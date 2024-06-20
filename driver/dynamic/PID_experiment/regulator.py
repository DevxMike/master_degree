import paho.mqtt.client as mqtt
import json
from enum import Enum
import time
import math

class RobotTopicsMappingIn(Enum):
    DEBUG_TOPIC = 0
    COMMAND_TOPIC = 1
    SET_MOTORS_TOPIC = 2

class RobotCommandsMapping(Enum):
    RESET_ODO = 1
    GET_ALL = 3
    HALT = 4

class RobotTopicsMappingOut(Enum):
    DEBUG_TOPIC = 0
    COMMAND_RESPONSE_TOPIC = 1

robot_topics_out = [
    'robot/echo/out',
    'robot/cmd/response'
]

robot_topics_in = [
    'robot/echo/in',
    'robot/cmd/in',
    'robot/set/motors'
]

robot_commands = [
    'get_sensors',
    'reset_odo',
    'get_odo',
    'get_all',
    'halt'
]

import os
import csv

odometry = {
    'x' : float(0),
    'y' : float(0),
    'theta' : float(0)
}

initial_odometry = {
    'x' : float(0),
    'y' : float(0),
    'theta' : float(0)
}

sensor_readings = {
    'front_left' : float(0),
    'front' : float(0),
    'front_right' : float(0),
    'rear' : float(0)
}

def on_message_cback(client, data, message):
    # print(f'{message.timestamp} {message.topic} : {message.payload.decode()}')
    if(message.topic == robot_topics_out[RobotTopicsMappingOut.COMMAND_RESPONSE_TOPIC.value]):
        try:
            payload = json.loads(message.payload.decode())
            timestamp = message.timestamp
            
            odometry['x'] = payload['x']
            odometry['y'] = payload['y']
            odometry['theta'] = payload['theta']

            sensor_readings['front_left'] = payload['front_left']
            sensor_readings['front_right'] = payload['front_right']
            sensor_readings['front'] = payload['front']
            sensor_readings['rear'] = payload['rear']

            print(f'{timestamp} odometry status: {odometry}, sensor status: {sensor_readings}')
        except:
            pass


def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to broker")
        for t in robot_topics_out:  # sub robot out topics
            client.subscribe(t)
    else:
        print(f"Connection failed with code {rc}")

def on_disconnect(client, userdata, rc):
    if rc != 0:
        print(f"Unexpected disconnection: {rc}")
    else:
        print("Disconnected from broker")

BROKER = "mqtt-dashboard.com"
PORT = 1883

MQTT_client = mqtt.Client("myRobotApp")
MQTT_client.on_message = on_message_cback
MQTT_client.on_connect = on_connect
MQTT_client.on_disconnect = on_disconnect

MQTT_client.connect(BROKER, PORT) # connect to broker

MQTT_client.loop_start()

time.sleep(5)

def publish_raw(client, topic: Enum, payload):
    t = robot_topics_in[topic.value]
    print(f"publishing {t} {payload}")
    client.publish(t, payload)

def publish_cmd(client, cmd: Enum):
    c = robot_commands[cmd.value]
    t = robot_topics_in[RobotTopicsMappingIn.COMMAND_TOPIC.value]
    print(f"publishing {t} {c}")
    client.publish(t, c)

MQTT_client.publish('robot/debug/out', 'PID application running')

publish_cmd(MQTT_client, RobotCommandsMapping.RESET_ODO)
# publish_raw(
#     MQTT_client,
#     RobotTopicsMappingIn.SET_MOTORS_TOPIC,
#     json.dumps({ "left" : 40, "right" : 40})
# )

class PDController:
    def __init__(self, Kp, Kd):
        self.Kp = Kp
        self.Kd = Kd
        self.previous_error = 0
        self.previous_time = time.time()

    def update(self, setpoint, current_value):
        current_time = time.time()
        dt = current_time - self.previous_time
        error = setpoint - current_value

        if abs(error) <= 0.05:
            return 0

        if dt == 0:
            derivative = 0
        else:
            derivative = (error - self.previous_error) / dt

        output = self.Kp * error + self.Kd * derivative

        self.previous_error = error
        self.previous_time = current_time

        return output
        

def calculate_distance():
    dx = odometry['x'] - initial_odometry['x']
    dy = odometry['y'] - initial_odometry['y']
    return math.sqrt(dx**2 + dy**2)

def control_drive(distance_to_go, pd_controller):
    current_distance = calculate_distance()
    
    if (odometry['x'] - initial_odometry['x']) < 0:
        current_distance = -current_distance

    speed_command = pd_controller.update(distance_to_go, current_distance)

    if speed_command > 0:
        print(f"Driving forward with speed: {speed_command}")
        publish_raw(
            MQTT_client,
            RobotTopicsMappingIn.SET_MOTORS_TOPIC,
            json.dumps({"left": 40, "right": 40})
        )
    elif speed_command < 0:
        print(f"Driving backward with speed: {abs(speed_command)}")
        publish_raw(
            MQTT_client,
            RobotTopicsMappingIn.SET_MOTORS_TOPIC,
            json.dumps({"left": -40, "right": -40})
        )
    else:
        print("Robot has reached the target distance.")
        publish_raw(
            MQTT_client,
            RobotTopicsMappingIn.SET_MOTORS_TOPIC,
            json.dumps({"left": 0, "right": 0})
        )

def initialize_odometry():
    global initial_odometry
    initial_odometry = odometry.copy()

Kp = 1.0
Kd = 0.1

distance_to_go = -1.0  
pd = PDController(Kp, Kd)
initialize_odometry()

try:
    while True:
        control_drive(distance_to_go, pd)
        publish_cmd(MQTT_client, RobotCommandsMapping.GET_ALL)
        time.sleep(0.1)
except KeyboardInterrupt:
    publish_raw(
        MQTT_client,
        RobotTopicsMappingIn.SET_MOTORS_TOPIC,
        json.dumps({ "left" : 0, "right" : 0})
    )
    print("Disconnecting...")
    MQTT_client.loop_stop()
    MQTT_client.disconnect()