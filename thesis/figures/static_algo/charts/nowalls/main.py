import matplotlib.pyplot as plt

# Dane z tabel
graph_sizes = ['200x50', '200x200', '500x500', '1000x1000', '2000x2000']
manhattan_times = [0.0762, 0.0889, 0.2118, 0.7892, 3.0142]
euclidean_times = [0.0952, 0.1070, 0.2364, 0.8041, 3.0193]
max_times = [0.0964, 0.1067, 0.2229, 0.7815, 3.0381]
dijkstra_times = [0.0171, 0.0593, 0.1247, 0.3985, 1.5084]
best_astar_times = [0.0762, 0.0889, 0.2118, 0.7815, 3.0142]

manhattan_times_ms = [time * 1000 for time in manhattan_times]
euclidean_times_ms = [time * 1000 for time in euclidean_times]
max_times_ms = [time * 1000 for time in max_times]
dijkstra_times_ms = [time * 1000 for time in dijkstra_times]
best_astar_times_ms = [time * 1000 for time in best_astar_times]

# Wykres czasu działania algorytmu A* dla różnych heurystyk
plt.figure(figsize=(10, 6))
plt.plot(graph_sizes, manhattan_times_ms, label='Manhattan', marker='o')
plt.plot(graph_sizes, euclidean_times_ms, label='Euclidean', marker='o')
plt.plot(graph_sizes, max_times_ms, label='Max', marker='o')
plt.xlabel('Rozmiary grafu [-]')
plt.ylabel('Czas [ms]')
plt.title('Czas działania algorytmu A* dla różnych metryk')
plt.legend()
plt.grid(True)
plt.savefig('astar_heuristics.png')
plt.close()

# Wykres czasu działania algorytmu Dijkstry
plt.figure(figsize=(10, 6))
plt.plot(graph_sizes, dijkstra_times_ms, label='Dijkstra', color='orange', marker='o')
plt.xlabel('Rozmiary grafu [-]')
plt.ylabel('Czas [ms]')
plt.title('Czas działania algorytmu Dijkstry')
plt.legend()
plt.grid(True)
plt.savefig('dijkstra.png')
plt.close()

# Wykres porównujący najlepsze wyniki A* z wynikami Dijkstry
plt.figure(figsize=(10, 6))
plt.plot(graph_sizes, best_astar_times_ms, label='Najlepszy czas A*', marker='o')
plt.plot(graph_sizes, dijkstra_times_ms, label='Czas Dijkstry', marker='o')
plt.xlabel('Rozmiary grafu [-]')
plt.ylabel('Czas [ms]')
plt.title('Porównanie najlepszych wyników A* i Dijkstry')
plt.legend()
plt.grid(True)
plt.savefig('astar_vs_dijkstra.png')
plt.close()

