import numpy as np
import pygad
import paho.mqtt.client as mqtt
import json
import time

# Parametry MQTT
MQTT_BROKER = "mqtt-dashboard.com"
MQTT_PORT = 1883
MQTT_TOPIC_PID = "robot/set/pid"
MQTT_TOPIC_MOTORS = "robot/set/motors"
MQTT_TOPIC_LOG = "robot/pid/log"
MQTT_TOPIC_RESPONSE = "robot/request"

# Inicjalizacja klienta MQTT
mqtt_client = mqtt.Client("PID_Controller")
mqtt_client.connect(MQTT_BROKER, MQTT_PORT)

# Zmienne do przechowywania logów
logs = []

# Funkcja wysyłająca wiadomość MQTT
def send_mqtt_message(topic, message):
    mqtt_client.publish(topic, json.dumps(message))

# Funkcja przeprowadzająca eksperyment
def run_experiment(Kp, Ti, Td, Ts):
    # Wysłanie nastaw PID
    pid_settings = {"Kp": Kp, "Ti": Ti, "Td": Td, "Ts": Ts}
    send_mqtt_message(MQTT_TOPIC_PID, pid_settings)

    # Przykładowe dane o prędkościach silników
    motors_speed = {"left": 50, "right": 0}

    # Wysłanie danych o prędkościach silników
    send_mqtt_message(MQTT_TOPIC_MOTORS, motors_speed)

    # Czekanie na 5 sekund na odpowiedź
    start_time = time.time()
    while time.time() - start_time < 5:
        time.sleep(1)  # Oczekiwanie na dane

        # Sprawdzenie logów z regulacji PID
        for log in logs:
            target = log["target"]
            actual = log["actual"]
            error = abs(actual - target)
            if -2 <= error <= 2:
                settling_time = log["id"]
                error_percentage = error / target * 100  # Obliczenie błędu w procentach
                # Tutaj można dodać logikę obliczania czasu osiągnięcia przedziału (-2%, 2%)
                # np. aktualizować wartość settling_time na podstawie kolejnych logów
                # Zakończenie eksperymentu po osiągnięciu warunków stopu
                if settling_time >= 0 and error_percentage <= 2:
                    send_mqtt_message(MQTT_TOPIC_MOTORS, {"left": 0, "right": 0})
                    send_mqtt_message(MQTT_TOPIC_RESPONSE, "Zakończono eksperyment")
                    return error_percentage, settling_time

# Funkcja obsługująca logi z regulacji PID
def on_log_message(client, userdata, message):
    log_data = json.loads(message.payload.decode())
    logs.append(log_data)

# Subskrypcja tematu z logami
mqtt_client.subscribe(MQTT_TOPIC_LOG)
mqtt_client.on_message = on_log_message

# Parametry algorytmu genetycznego
def run_genetic_algorithm(pop_size, max_epochs, crossover_prob, num_parents_mating, mutation_prob):
    # Definicja problemu optymalizacyjnego
    num_genes = 3  # Liczba genów w chromosomie (liczba parametrów PID)
    gene_space = [{'low': 0, 'high': 40}]*num_genes  # Przestrzeń poszukiwań dla każdego parametru PID

    # Inicjalizacja obiektu problemu
    ga_instance = pygad.GA(num_generations=max_epochs,
                           num_parents_mating=num_parents_mating,
                           sol_per_pop=pop_size,
                           num_genes=num_genes,
                           gene_space=gene_space,
                           crossover_probability=crossover_prob,
                           mutation_percent_genes=mutation_prob,
                           fitness_func=run_experiment)

    # Uruchomienie algorytmu genetycznego
    ga_instance.run()

    # Zwrócenie najlepszego rozwiązania
    best_solution = ga_instance.best_solution()
    return best_solution

# Parametry eksperymentów
experiments_params = [
    {"pop_size": 100, "max_epochs": 10000, "crossover_prob": 0.5, "num_parents_mating": 2, "mutation_prob": 0.02},
    {"pop_size": 100, "max_epochs": 10000, "crossover_prob": 0.33, "num_parents_mating": 3, "mutation_prob": 0.03}
]

# Pętla przeprowadzająca eksperymenty
for params in experiments_params:
    best_solution = run_genetic_algorithm(params["pop_size"], params["max_epochs"], params["crossover_prob"],
                                          params["num_parents_mating"], params["mutation_prob"])

# Oczekiwanie na wiadomości MQTT
mqtt_client.loop_forever()