import paho.mqtt.client as mqtt
import json
from enum import Enum
import time

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

experiment_no = 1
log_files = ['straight_drive.csv', 'turning.csv']
velocities = [{ "left" : 40, "right" : 40}, { "left" : 40, "right" : -40}]

def on_message_cback(client, data, message):
    # print(f'{message.timestamp} {message.topic} : {message.payload.decode()}')
    if(message.topic == robot_topics_out[RobotTopicsMappingOut.COMMAND_RESPONSE_TOPIC.value]):
        payload = json.loads(message.payload.decode())
        timestamp = message.timestamp

        log_entry = {
            'timestamp': timestamp,
            **payload
        }

        file_exists = os.path.isfile(log_files[experiment_no])

        with open(log_files[experiment_no], 'a') as log_file:
            fieldnames = ['timestamp', 'x', 'y', 'theta', 'front_left', 'front', 'front_right', 'rear']
            writer = csv.DictWriter(log_file, fieldnames=fieldnames)

            if not file_exists:
                writer.writeheader()

            writer.writerow(log_entry)

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

for t in robot_topics_out: # sub robot out topics
    MQTT_client.subscribe(t)

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
publish_raw(
    MQTT_client,
    RobotTopicsMappingIn.SET_MOTORS_TOPIC,
    json.dumps(velocities[experiment_no])
)



try:
    while True:
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