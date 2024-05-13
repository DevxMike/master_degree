import tkinter as tk

class ControlGUI:
    def __init__(self, master, mqtt_client):
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

        self.manual_auto_var = tk.StringVar()
        self.manual_auto_var.set("Manual")
        self.manual_auto_button = tk.Button(master, textvariable=self.manual_auto_var, command=self.toggle_manual_auto)
        self.manual_auto_button.pack()

        self.mqtt_client = mqtt_client

        # Manual Control Variables
        self.manual_control = True

        # Binding Key Events
        self.master.bind("<KeyPress>", self.key_pressed_handler)
        self.master.bind("<KeyRelease>", self.key_released_handler)

    def toggle_manual_auto(self):
        if self.manual_auto_var.get() == "Manual":
            self.manual_auto_var.set("Auto")
            self.manual_control = False
        else:
            self.manual_auto_var.set("Manual")
            self.manual_control = True

    def key_pressed_handler(self, event):
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

    def update_robot_parameters(self, x, y, theta):
        self.label_x.config(text=f"X: {x:.2f}")
        self.label_y.config(text=f"Y: {y:.2f}")
        self.label_theta.config(text=f"Theta: {theta:.2f}")