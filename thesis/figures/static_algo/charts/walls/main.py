import matplotlib.pyplot as plt

# Dane z tabeli
iterations = [0, 1, 2, 3, 4]
astar_manhattan_times = [0.1140, 0.1030, 0.1224, 0.0568, 0.0854]
astar_euclidean_times = [0.1030, 0.1216, 0.1309, 0.1048, 0.0923]
astar_max_times = [0.0991, 0.1197, 0.1425, 0.1020, 0.0984]
dijkstra_times = [0.0234, 0.0169, 0.0187, 0.0184, 0.0182]

astar_manhattan_times_ms = [time * 1000 for time in astar_manhattan_times]
astar_euclidean_times_ms = [time * 1000 for time in astar_euclidean_times]
astar_max_times_ms = [time * 1000 for time in astar_max_times]
dijkstra_times_ms = [time * 1000 for time in dijkstra_times]

astar_manhattan_lengths = [247, 243, 247, 233, 243]
astar_euclidean_lengths = [247, 243, 247, 233, 243]
astar_max_lengths = [247, 243, 247, 233, 243]
dijkstra_lengths = [247, 243, 247, 233, 243]

# Wykres czasu działania algorytmów
plt.figure(figsize=(10, 6))
plt.plot(iterations, astar_manhattan_times_ms, label='A* Manhattan', marker='o')
plt.plot(iterations, astar_euclidean_times_ms, label='A* Euclidean', marker='o')
plt.plot(iterations, astar_max_times_ms, label='A* Max', marker='o')
plt.plot(iterations, dijkstra_times_ms, label='Dijkstra', marker='o')
plt.xlabel('Iteracja')
plt.ylabel('Czas działania algorytmu [ms]')
plt.title('Czas działania algorytmów w różnych iteracjach')
plt.legend()
plt.grid(True)
plt.savefig('algorithm_times.png')
plt.close()

# Wykres długości trasy algorytmów
plt.figure(figsize=(10, 6))
plt.plot(iterations, astar_manhattan_lengths, label='A* Manhattan', marker='o')
plt.plot(iterations, astar_euclidean_lengths, label='A* Euclidean', marker='o')
plt.plot(iterations, astar_max_lengths, label='A* Max', marker='o')
plt.plot(iterations, dijkstra_lengths, label='Dijkstra', marker='o')
plt.xlabel('Iteracja')
plt.ylabel('Długość trasy')
plt.title('Długość trasy w różnych iteracjach')
plt.legend()
plt.grid(True)
plt.savefig('route_lengths.png')
plt.close()

