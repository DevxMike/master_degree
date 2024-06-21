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
    def __init__(self, Kp, Kd, deadzone):
        self.Kp = Kp
        self.Kd = Kd
        self.previous_error = 0
        self.previous_time = time.time()
        self.deadzone = deadzone

    def update(self, setpoint, current_value):
        current_time = time.time()
        dt = current_time - self.previous_time
        error = setpoint - current_value

        if abs(error) <= self.deadzone:
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

def control_drive(distance_to_go, pd_controller, direction):
    current_distance = calculate_distance()

    if direction == 'backward':
        current_distance = -current_distance

    speed_command = pd_controller.update(distance_to_go, current_distance)

    if speed_command > 0:
        if direction == 'forward':
            print(f"Driving forward with speed: {speed_command}")
            publish_raw(
                MQTT_client,
                RobotTopicsMappingIn.SET_MOTORS_TOPIC,
                json.dumps({"left": 43, "right": 40})
            )
        else:  # direction == 'backward'
            print(f"Driving backward with speed: {abs(speed_command)}")
            publish_raw(
                MQTT_client,
                RobotTopicsMappingIn.SET_MOTORS_TOPIC,
                json.dumps({"left": -43, "right": -40})
            )

        return False
    
    elif speed_command < 0:
        if direction == 'forward':
            print(f"Driving backward with speed: {abs(speed_command)}")
            publish_raw(
                MQTT_client,
                RobotTopicsMappingIn.SET_MOTORS_TOPIC,
                json.dumps({"left": -43, "right": -40})
            )
        else:  # direction == 'backward'
            print(f"Driving forward with speed: {speed_command}")
            publish_raw(
                MQTT_client,
                RobotTopicsMappingIn.SET_MOTORS_TOPIC,
                json.dumps({"left": 43, "right": 40})
            )
    
        return False
    
    else:
        print("Robot has reached the target distance.")
        publish_raw(
            MQTT_client,
            RobotTopicsMappingIn.SET_MOTORS_TOPIC,
            json.dumps({"left": 0, "right": 0})
        )

        return True

def control_theta(theta_to_go, pd_controller):
    current_theta = odometry['theta'] - initial_odometry['theta']
    theta_command = pd_controller.update(theta_to_go, current_theta)

    if theta_command > 0:
        print(f"Turning clockwise with speed: {theta_command}")
        publish_raw(
            MQTT_client,
            RobotTopicsMappingIn.SET_MOTORS_TOPIC,
            json.dumps({"left": -43, "right": 40})
        )

        return False
    
    elif theta_command < 0:
        print(f"Turning counter-clockwise with speed: {abs(theta_command)}")
        publish_raw(
            MQTT_client,
            RobotTopicsMappingIn.SET_MOTORS_TOPIC,
            json.dumps({"left": 43, "right": -40})
        )

        return False
    
    else:
        print("Robot has reached the target theta.")
        publish_raw(
            MQTT_client,
            RobotTopicsMappingIn.SET_MOTORS_TOPIC,
            json.dumps({"left": 0, "right": 0})
        )

        return True

def initialize_odometry():
    global initial_odometry
    initial_odometry = odometry.copy()

Kp = 1.0
Kd = 0.1

distance_to_go = 1.0  
pd = PDController(Kp, Kd, 0.1)

def deg_to_rad(degrees):
    return degrees * (math.pi / 180)

def get_theta(degrees):
    return deg_to_rad(degrees) / 1.1

ROTATION = 1
STRAIGHT_DRIVE = 2

def create_task(magnitude, ROT_OR_DRIVE, drive_direction='forward'):
    mode = ROT_OR_DRIVE
    fn = None

    if(mode == ROTATION):
        return (magnitude, ROTATION, lambda pd_ctl : control_theta(magnitude, pd_ctl))
    elif(mode == STRAIGHT_DRIVE):
        return (magnitude, mode, lambda pd_ctl : control_drive(magnitude, pd_ctl, drive_direction))


# task_list = [
#     (distance_to_go, STRAIGHT_DRIVE, lambda pd_ctl : control_drive(distance_to_go, pd_ctl)),
#     (theta_to_go, ROTATION, lambda pd_ctl : control_theta(theta_to_go, pd_ctl))
# ]

task_list = [
    create_task(1.0, STRAIGHT_DRIVE, 'forward'),
    create_task(get_theta(180), ROTATION),
    create_task(0.5, STRAIGHT_DRIVE, 'forward')
]

i = 0

initialize_odometry()

try:
    while True:
        publish_cmd(MQTT_client, RobotCommandsMapping.GET_ALL)
        _, mode, task = task_list[i]
        if(task(pd)):
            i = i + 1
            initialize_odometry()

            if(i >= len(task_list)):
                break
        time.sleep(0.1)
        
        if(mode == ROTATION):
            publish_raw(
                MQTT_client,
                RobotTopicsMappingIn.SET_MOTORS_TOPIC,
                json.dumps({ "left" : 0, "right" : 0})
            )
            time.sleep(0.03)

except KeyboardInterrupt:
    publish_raw(
        MQTT_client,
        RobotTopicsMappingIn.SET_MOTORS_TOPIC,
        json.dumps({ "left" : 0, "right" : 0})
    )
    print("Disconnecting...")
    MQTT_client.loop_stop()
    MQTT_client.disconnect()

initialize_odometry()

# try:
#     while True:
#         publish_cmd(MQTT_client, RobotCommandsMapping.GET_ALL)
#         if(control_theta(theta_to_go, pd)):
#             break
#         time.sleep(0.1)
#         publish_raw(
#             MQTT_client,
#             RobotTopicsMappingIn.SET_MOTORS_TOPIC,
#             json.dumps({"left": 0, "right": 0})
#         )
#         time.sleep(.02)
# except KeyboardInterrupt:
#     publish_raw(
#         MQTT_client,
#         RobotTopicsMappingIn.SET_MOTORS_TOPIC,
#         json.dumps({ "left" : 0, "right" : 0})
#     )
#     print("Disconnecting...")
#     MQTT_client.loop_stop()
#     MQTT_client.disconnect()

publish_raw(
        MQTT_client,
        RobotTopicsMappingIn.SET_MOTORS_TOPIC,
        json.dumps({ "left" : 0, "right" : 0})
    )

print("Disconnecting...")
MQTT_client.loop_stop()
MQTT_client.disconnect()