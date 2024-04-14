from paho.mqtt import client as paho
import random

broker = "mqtt-dashboard.com"
port = 1883
logTopic = "robot/pid/log"
client_id = f'python-mqtt-{random.randint(0, 1000)}'

import paho.mqtt.client as mqtt

# Funkcja obsługująca zdarzenie połączenia z brokerem MQTT
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Pomyślnie połączono z brokerem MQTT")
        # Subskrybuj się do wybranego tematu po nawiązaniu połączenia
        client.subscribe(logTopic)
    else:
        print("Błąd połączenia z brokerem MQTT")

# Funkcja obsługująca otrzymane wiadomości MQTT
def on_message(client, userdata, message):
    print("Otrzymano wiadomość: ", message.payload.decode())

# Konfiguracja klienta MQTT
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

# Podłącz się do brokera MQTT (np. Mosquitto) na lokalnym hoście
# Ustaw adres IP lub nazwę hosta brokera MQTT oraz port (domyślnie 1883)
client.connect(broker, port, 60)

# Rozpocznij nasłuchiwanie na wiadomości
client.loop_forever()