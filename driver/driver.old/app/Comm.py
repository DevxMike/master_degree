import paho.mqtt.client as mqtt
import json
from enum import Enum

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
    'robo/set/motors'
]

robot_commands = [
    'get_sensors',
    'reset_odo',
    'get_odo',
    'get_all',
    'halt'
]

class MQTTCommunication:
    def __init__(self, broker, port, topics, on_message):
        self.client = mqtt.Client("RobotControlApp")
        self.client.connect(broker, port)
        
        for t in topics:
            self.client.subscribe(t)

        self.client.on_message = on_message
        self.client.loop_start()
        self.client.publish('robot/debug', 'application running')

    def send_message(self, topic, message):
        json_message = json.dumps(message, ensure_ascii=False)  # Konwersja słownika na ciąg JSON
        self.client.publish(topic, json_message)

    def send_command(self, command : Enum):
        if isinstance(command, RobotCommandsMapping):
            self.client.publish(
                robot_topics_in[RobotTopicsMappingIn.COMMAND_TOPIC.value],
                robot_commands[command.value]
            )
            return 0
        
        else:
            return -1
    