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
mqtt_client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
mqtt_client.connect(MQTT_BROKER, MQTT_PORT)
mqtt_client.subscribe(MQTT_TOPIC_LOG)

logs = []

def on_log_message(client, userdata, message):
    global logs 
    log_data = json.loads(message.payload.decode())
    logs.append(log_data)  

mqtt_client.on_message = on_log_message

# Funkcja wysyłająca wiadomość MQTT
def send_mqtt_message(topic, message):
    mqtt_client.publish(topic, json.dumps(message))

# Funkcja uruchamiająca eksperyment
def run_experiment(Kp, Ti, Td):
    pid_settings = {"Kp": Kp, "Ti": Ti, "Td": Td}
    send_mqtt_message(MQTT_TOPIC_PID, pid_settings)

    motors_speed = {"left": 50, "right": 0}
    send_mqtt_message(MQTT_TOPIC_MOTORS, motors_speed)

    start_time = time.time()
    while time.time() - start_time < 5:
        mqtt_client.loop()

    motors_speed = {"left": 0, "right": 0}
    send_mqtt_message(MQTT_TOPIC_MOTORS, motors_speed)

    time.sleep(2)
    
    send_mqtt_message(MQTT_TOPIC_RESPONSE, "")

# Funkcja obliczająca błędy
def calculate_errors(logs):
    absolute_errors = []
    relative_errors = []
    settling_times = []

    sampling_time = 5

    # print(logs)

    for i in range(len(logs)):
        measurement = logs[i]
        absolute_error = abs(measurement['target'] - measurement['actual'])
        relative_error = (absolute_error / measurement['target']) * 100 if measurement['target'] != 0 else 0
        absolute_errors.append(absolute_error)
        relative_errors.append(relative_error)
        
        # Sprawdzenie, czy wartość docelowa została osiągnięta
        if relative_error < 2:  # Możesz dostosować wartość graniczną według potrzeb
            settling_time = i * sampling_time
            settling_times.append(settling_time)

    mean_error = sum(absolute_errors) / len(absolute_errors)
    mean_relative_error = sum(relative_errors) / len(relative_errors)
    rmse_error = math.sqrt(sum([(error ** 2) for error in absolute_errors]) / len(absolute_errors))
    average_settling_time = sum(settling_times) / len(settling_times)

    return mean_relative_error, average_settling_time

# Funkcja obliczająca wartość funkcji celu
def fitness_func(ga_instance, solution, solution_idx):
    global logs

    Kp, Ti, Td = solution
    run_experiment(Kp, Ti, Td)

    mean_relative_error, average_settling_time = calculate_errors(logs)
    logs = []

    # Im mniejsza wartość funkcji celu, tym lepsze parametry PID
    # W tym przypadku minimalizujemy błąd względny i czas osiągnięcia wartości docelowej
    fitness = 1 / (mean_relative_error + average_settling_time)
    return fitness

# Funkcja zapisująca najlepszego osobnika do pliku
def save_best_solution(ga_instance, experiment_num):
    best_solution = ga_instance.best_solution()
    best_fitness = best_solution[1]
    generation_num = ga_instance.generations_completed
    filename = f"best_solution_experiment_{experiment_num}.txt"
    with open(filename, "a") as file:
        file.write(f"Generation {generation_num}: Best solution: {best_solution}, Fitness: {best_fitness}\n")
    print(f"Generation {generation_num} saved to {filename}")

# Funkcja uruchamiająca algorytm genetyczny dla danego zestawu parametrów
def run_genetic_algorithm(pop_size, max_epochs, crossover_prob, num_parents_mating, mutation_prob, experiment_num):
    # Definicja problemu optymalizacyjnego
    num_genes = 3  # Liczba genów w chromosomie (liczba parametrów PID)
    gene_space = [{'low': 0, 'high': 50}]*num_genes  # Przestrzeń poszukiwań dla każdego parametru PID

    on_generation = lambda x: save_best_solution(x, experiment_num)
    # Inicjalizacja obiektu problemu
    ga_instance = pygad.GA(num_generations=max_epochs,
                           num_parents_mating=num_parents_mating,
                           sol_per_pop=pop_size,
                           num_genes=num_genes,
                           gene_space=gene_space,
                           crossover_probability=crossover_prob,
                           mutation_percent_genes='default',
                           mutation_probability=mutation_prob,
                           fitness_func=fitness_func,
                           on_generation=on_generation)

    # Uruchomienie algorytmu genetycznego
    ga_instance.run()
    best_solution = ga_instance.best_solution()
    return best_solution

# Parametry eksperymentów
experiments_params = [
    {"pop_size": 25, "max_epochs": 100, "crossover_prob": 0.5, "num_parents_mating": 2, "mutation_prob": 0.02},
    {"pop_size": 25, "max_epochs": 100, "crossover_prob": 0.33, "num_parents_mating": 5, "mutation_prob": 0.03}
]

# Pętla przeprowadzająca eksperymenty
for i, params in enumerate(experiments_params):
    print(run_genetic_algorithm(params["pop_size"], params["max_epochs"], params["crossover_prob"],
                          params["num_parents_mating"], params["mutation_prob"], i+1))
