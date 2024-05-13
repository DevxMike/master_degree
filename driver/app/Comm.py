import paho.mqtt.client as mqtt
import json

class MQTTCommunication:
    def __init__(self, broker, port, topic):
        self.client = mqtt.Client("RobotControlApp")
        self.client.connect(broker, port)
        self.client.subscribe(topic)
        self.client.on_message = self.on_message
        self.client.loop_start()
        self.client.publish('robot/debug', 'application running')

    def send_message(self, topic, message):
        json_message = json.dumps(message, ensure_ascii=False)  # Konwersja słownika na ciąg JSON
        self.client.publish(topic, json_message)

    def on_message(self, client, userdata, msg):
        # Process received messages
        pass
