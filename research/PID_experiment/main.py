import paho.mqtt.client as mqtt
import json
import time
import math
import numpy as np
import pygad

# Konfiguracja MQTT
MQTT_BROKER = "mqtt-dashboard.com"
MQTT_PORT = 1883
MQTT_TOPIC_PID = "robot/set/pid"
MQTT_TOPIC_MOTORS = "robot/set/motors"
MQTT_TOPIC_LOG = "robot/pid/log"
MQTT_TOPIC_RESPONSE = "robot/request"

# Połączenie z brokerem MQTT
mqtt_client = mqtt.Client("PID Ctl")

logs = []
experiment_ongoing = False

def on_log_message(client, userdata, message):
    global experiment_ongoing
    log_data = json.loads(message.payload.decode())
    if len(logs) != 0: 
        logs.append(log_data) 
    elif len(logs) == 0 and log_data["id"] == 0:
        logs.append(log_data)

# Funkcja wysyłająca wiadomość MQTT
def send_mqtt_message(topic, message):
    mqtt_client.publish(topic, json.dumps(message))

# Funkcja uruchamiająca eksperyment
def run_experiment(Kp, Ti, Td):
    global logs
    global mqtt_client
    global experiment_ongoing
    mqtt_client = mqtt.Client("PID ctl")
    mqtt_client.connect(MQTT_BROKER, MQTT_PORT)
    mqtt_client.subscribe(MQTT_TOPIC_LOG)
    mqtt_client.on_message = on_log_message

    mqtt_client.loop()
    logs = []

    pid_settings = {"Kp": Kp, "Ti": Ti, "Td": Td}
    send_mqtt_message(MQTT_TOPIC_PID, pid_settings)

    motors_speed = {"left": 0, "right": 50}
    send_mqtt_message(MQTT_TOPIC_MOTORS, motors_speed)
    experiment_ongoing = True
    start_time = time.time()
    while time.time() - start_time < 2:
        mqtt_client.loop()

    motors_speed = {"left": 0, "right": 0}
    send_mqtt_message(MQTT_TOPIC_MOTORS, motors_speed)

    start_time = time.time()

    while time.time() - start_time < 1:
        mqtt_client.loop()
    
    send_mqtt_message(MQTT_TOPIC_RESPONSE, "")
    experiment_ongoing = False

# Funkcja obliczająca błędy
def calculate_errors(logs):
    absolute_errors = []
    relative_errors = []

    for i in range(len(logs)):
        measurement = logs[i]
        absolute_error = abs(measurement['target'] - measurement['actual'])
        
        if measurement['target'] != 0:
            relative_error = (absolute_error / measurement['target']) * 100
        else:
            if measurement['actual'] == measurement['target']:
                relative_error = 0
            else:
                relative_error = float(100)  
        
        absolute_errors.append(absolute_error)
        relative_errors.append(relative_error)

    mean_relative_error = (sum(relative_errors) / len(relative_errors)) if(len(relative_errors)) != 0 else 10000

    if(mean_relative_error) == 0.0:
        return 10000

    return mean_relative_error

from datetime import datetime
import csv 

def generate_file_title():
    now = datetime.now()
    return f"logs_{now.strftime('%Y-%m-%d_%H-%M')}"

def create_csv_header(file_path):
    with open(file_path, mode='w', newline='') as csv_file:
        writer = csv.writer(csv_file)
        writer.writerow(["Ki", "Ti", "Td", "error", "generations"])

def append_data_to_csv(file_path, data):
    with open(file_path, mode='a', newline='') as csv_file:
        writer = csv.writer(csv_file)
        writer.writerow(data)

log_file = ""



# Funkcja obliczająca wartość funkcji celu
def fitness_func(ga_instance, solution, solution_idx):
    global logs
    global log_file

    Kp, Ti, Td = solution
    run_experiment(Kp, Ti, Td)

    mean_relative_error = calculate_errors(logs)
    logs = []
    append_data_to_csv(log_file, [Kp, Ti, Td, mean_relative_error, ga_instance.generations_completed])
    print(mean_relative_error)

    # Im mniejsza wartość funkcji celu, tym lepsze parametry PID
    # W tym przypadku minimalizujemy błąd względny i czas osiągnięcia wartości docelowej
    fitness = 1 / (mean_relative_error + 0.001)
    return fitness

# Funkcja zapisująca najlepszego osobnika do pliku
def save_best_solution(ga_instance, experiment_num):
    best_solution = ga_instance.best_solution(pop_fitness=ga_instance.last_generation_fitness)
    best_fitness = ga_instance.best_solution(pop_fitness=ga_instance.last_generation_fitness)[1]
    generation_num = ga_instance.generations_completed
    filename = f"best_solution_experiment_{experiment_num}_mutation.txt"
    with open(filename, "a") as file:
        file.write(f"Generation {generation_num}: Best solution: {best_solution}, Fitness: {best_fitness}\n")
    print(f"Generation {generation_num} saved to {filename}")

    t = time.time()
    while time.time() - t < 60:
        continue

# Funkcja uruchamiająca algorytm genetyczny dla danego zestawu parametrów
def run_genetic_algorithm(pop_size, max_epochs, crossover_prob, num_parents_mating, mutation_prob, experiment_num):
    # Definicja problemu optymalizacyjnego
    num_genes = 3  # Liczba genów w chromosomie (liczba parametrów PID)

    on_generation = lambda x: save_best_solution(x, experiment_num)
    # Inicjalizacja obiektu problemu
    ga_instance = pygad.GA(num_generations=max_epochs,
                           num_parents_mating=num_parents_mating,
                           sol_per_pop=pop_size,
                           num_genes=num_genes,
                           gene_space={'low': 0, 'high': 30},
                           crossover_probability=crossover_prob,
                           mutation_percent_genes='default',
                           mutation_probability=mutation_prob,
                           K_tournament=num_parents_mating,
                           fitness_func=fitness_func,
                           on_generation=on_generation,
                           mutation_type='scramble',
                           crossover_type="uniform",
                           parent_selection_type='tournament',
                           keep_parents=0,
                           gene_type=float,
                           random_mutation_min_val=10,
                           random_mutation_max_val=20
                           )

    # Uruchomienie algorytmu genetycznego
    ga_instance.run()
    # ga_instance.plot_fitness()
    
    best_solution = ga_instance.best_solution()
    return best_solution

# Parametry eksperymentów
experiments_params = [
    {"pop_size": 100, "max_epochs": 25, "crossover_prob": 0.70, "num_parents_mating": 10, "mutation_prob": 0.15},
    {"pop_size": 200, "max_epochs": 25, "crossover_prob": 0.70, "num_parents_mating": 15, "mutation_prob": 0.15},
    {"pop_size": 300, "max_epochs": 25, "crossover_prob": 0.70, "num_parents_mating": 25, "mutation_prob": 0.15}
]

# Pętla przeprowadzająca eksperymenty
for i, params in enumerate(experiments_params):
    log_file = generate_file_title()
    create_csv_header(log_file)
    print(run_genetic_algorithm(params["pop_size"], params["max_epochs"], params["crossover_prob"],
                          params["num_parents_mating"], params["mutation_prob"], i+1))
    t = time.time()
    while time.time() - t < 600:
        continue
