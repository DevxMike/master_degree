import matplotlib.pyplot as plt
import networkx as nx
import heapq
import time
import pickle
import math

def manhattan_distance(node, goal):
    return abs(node[0] - goal[0]) + abs(node[1] - goal[1])

def euclidean_distance(node, goal):
    return math.sqrt((node[0] - goal[0])**2 + (node[1] - goal[1])**2)

def max_distance(node, goal):
    return max(abs(node[0] - goal[0]), abs(node[1] - goal[1]))

def a_star(graph, start, goal, heuristic):
    """Implementacja algorytmu A*."""
    start_time = time.time()

    open_list = []
    heapq.heappush(open_list, (0, start))
    
    came_from = {}
    g_score = {node: float('inf') for node in graph}
    g_score[start] = 0
    
    f_score = {node: float('inf') for node in graph}
    f_score[start] = heuristic(start, goal)
    
    while open_list:
        _, current = heapq.heappop(open_list)
        
        if current == goal:
            path = []
            while current in came_from:
                path.append(current)
                current = came_from[current]
            path.append(start)
            path.reverse()
            end_time = time.time()
            print(f'Czas działania algorytmu A*: {end_time - start_time}s')
            return path
        
        for neighbor in graph[current]:
            tentative_g_score = g_score[current] + graph[current][neighbor]
            if tentative_g_score < g_score[neighbor]:
                came_from[neighbor] = current
                g_score[neighbor] = tentative_g_score
                f_score[neighbor] = g_score[neighbor] + heuristic(neighbor, goal)
                if neighbor not in [i[1] for i in open_list]:
                    heapq.heappush(open_list, (f_score[neighbor], neighbor))



def dijkstra(graph, start, goal):
    """Implementacja algorytmu Dijkstry."""
    start_time = time.time()
    queue = []
    heapq.heappush(queue, (0, start))
    
    distances = {node: float('inf') for node in graph}
    distances[start] = 0
    
    came_from = {}
    
    while queue:
        current_distance, current_node = heapq.heappop(queue)
        
        if current_node == goal:
            path = []
            while current_node in came_from:
                path.append(current_node)
                current_node = came_from[current_node]
            path.append(start)
            path.reverse()
            end_time = time.time()
            print(f'Czas działania algorytmu Dijkstry: {end_time - start_time}s')
            return path
        
        if current_distance > distances[current_node]:
            continue
        
        for neighbor, weight in graph[current_node].items():
            distance = current_distance + weight
            if distance < distances[neighbor]:
                distances[neighbor] = distance
                came_from[neighbor] = current_node
                heapq.heappush(queue, (distance, neighbor))

    return None

def draw_graph(graph, path=None):
    """
    Rysuje graf reprezentujący mapę pomieszczenia z uwzględnieniem optymalizacji wydajności.
    
    :param graph: Słownik reprezentujący graf (węzły jako klucze, sąsiedzi i odległości jako wartości).
    :param path: Lista węzłów reprezentująca ścieżkę do narysowania (opcjonalnie).
    """
    G = nx.Graph()
    
    for node, neighbors in graph.items():
        for neighbor, cost in neighbors.items():
            G.add_edge(node, neighbor, weight=cost)
    
    pos = {node: node for node in graph} 
    
    nx.draw_networkx_nodes(G, pos, node_size=2, node_color='lightblue')
    
    nx.draw_networkx_edges(G, pos, width=1.0)
    
    if len(graph) <= 100:
        edge_labels = nx.get_edge_attributes(G, 'weight')
        nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels)
    
    if path:
        path_edges = list(zip(path, path[1:]))
        nx.draw_networkx_edges(G, pos, edgelist=path_edges, edge_color='r', width=2)
    
    plt.axis('off') 
    plt.show()

from graph_ops import load_graph_from_file
graph_dims = [(200, 50), (200, 200), (500, 500), (1000, 1000), (2000, 2000)]
graphs = {}

for dim in graph_dims:
    fname = f'graph_{dim[0]}_{dim[1]}.pkl'
    graphs[fname] = load_graph_from_file(fname)

start = (0, 0)
goal = (198, 40)

heuristics={
    "manhattan" : manhattan_distance,
    "euclidean" : euclidean_distance,
    "max" : max_distance
}

for k, v in heuristics.items():
    print(f"Funkcja obliczająca odległość w algorytmie A*: {k}")
    for gname, graph in graphs.items():
        print(gname)
        path1 = a_star(graph, start, goal, v)
        path2 = dijkstra(graph, start, goal)
        print('\n')