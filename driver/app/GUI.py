import tkinter as tk
from Comm import MQTTCommunication
from Comm import robot_topics_out
from Comm import *
from utils import CallEventTimer
import math

class ControlGUI:
    def __init__(self, master, broker, port):
        self.master = master
        master.title("Robot Control App")

        # GUI Elements
        self.canvas = tk.Canvas(master, width=400, height=400, bg="white")
        self.canvas.pack()

        self.label_x = tk.Label(master, text="X:")
        self.label_x.pack()
        self.label_y = tk.Label(master, text="Y:")
        self.label_y.pack()
        self.label_theta = tk.Label(master, text="Theta:")
        self.label_theta.pack()
        self.label_left = tk.Label(master, text="Left:")
        self.label_left.pack()
        self.label_front = tk.Label(master, text="Front:")
        self.label_front.pack()
        self.label_right = tk.Label(master, text="Right:")
        self.label_right.pack()
        self.label_rear = tk.Label(master, text="Rear:")
        self.label_rear.pack()

        self.manual_auto_var = tk.StringVar()
        self.manual_auto_var.set("Manual")
        self.manual_auto_button = tk.Button(master, textvariable=self.manual_auto_var, command=self.toggle_manual_auto)
        self.manual_auto_button.pack()

        self.mqtt_client = MQTTCommunication(broker, port, robot_topics_out, self.on_message)

        # Manual Control Variables
        self.manual_control = True

        # Binding Key Events
        self.master.bind("<KeyPress>", self.key_pressed_handler)
        self.master.bind("<KeyRelease>", self.key_released_handler)

        self.event_poll = [
            CallEventTimer.CallEventTimer(
                300, lambda : self.mqtt_client.send_command(RobotCommandsMapping.GET_ALL)
            )
        ]

        for e in self.event_poll:
            e.start()

    def poll_events(self):
        for e in self.event_poll:
            e.poll()

        self.master.after(10, self.poll_events)
        pass

    def toggle_manual_auto(self):
        if self.manual_auto_var.get() == "Manual":
            self.manual_auto_var.set("Auto")
            self.manual_control = False
        else:
            self.manual_auto_var.set("Manual")
            self.manual_control = True

    def key_pressed_handler(self, event):
        if self.manual_control == True:
            key_pressed = event.keysym

            if(key_pressed == "Up"):
                self.mqtt_client.send_message('robot/set/motors', { "left" : 40, "right" : 40})
            elif(key_pressed == "Down"):
                self.mqtt_client.send_message('robot/set/motors', { "left" : -40, "right" : -40})
            elif(key_pressed == "Left"):
                self.mqtt_client.send_message('robot/set/motors', { "left" : -40, "right" : 40})
            elif(key_pressed == "Right"):
                self.mqtt_client.send_message('robot/set/motors', { "left" : 40, "right" : -40})
            elif(key_pressed == 'space'):
                self.mqtt_client.send_message('robot/set/motors', { "left" : 0, "right" : 0})

    def key_released_handler(self, event):
        # print('{ "left" : 0, "right" : 0}')
        pass

    def set_destination(self, event):
        # Set destination based on mouse click on the map
        pass

    def update_robot_parameters(self, x, y, theta, left, front, right, rear):
        theta_degrees = (theta * (180 / math.pi)) % 360

        self.label_x.config(text=f"X: {x:.2f}")
        self.label_y.config(text=f"Y: {y:.2f}")
        self.label_theta.config(text=f"Theta: {theta_degrees:.2f}")
        self.label_left.config(text=f'Left: {left} cm')
        self.label_rear.config(text=f'Rear: {rear} cm')
        self.label_front.config(text=f'Front: {front} cm')
        self.label_right.config(text=f'Right: {right} cm')

    def on_message(self, client, userdata, msg):
        print(f'{msg.timestamp} {msg.topic} : { msg.payload.decode("utf-8") }')

        if(msg.topic == robot_topics_out[RobotTopicsMappingOut.COMMAND_RESPONSE_TOPIC.value]):
            json_string = msg.payload.decode("utf-8")
            data = json.loads(json_string)
            self.update_robot_parameters(data['x'], data['y'], data['theta'], data['front_left'], data['front'], data['front_right'], data['rear'])


