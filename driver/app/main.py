from Comm import MQTTCommunication
from GUI import ControlGUI, tk

def main():
    broker = "mqtt-dashboard.com"
    port = 1883
    topic = "robot/control"

    mqtt_communication = MQTTCommunication(broker, port, topic)

    root = tk.Tk()
    gui = ControlGUI(root, mqtt_communication)

    # Additional initialization of Cruise Control, Navigation, etc.

    root.mainloop()

if __name__ == "__main__":
    main()
