import pickle
import matplotlib.pyplot as plt
import networkx as nx

def create_grid_graph(width, height):
    """
    Tworzy prostokątny graf o podanych wymiarach.
    
    :param width: Szerokość prostokąta (liczba kolumn).
    :param height: Wysokość prostokąta (liczba wierszy).
    :return: Graf w postaci słownika.
    """
    graph = {}
    
    for y in range(height):
        for x in range(width):
            graph[(x, y)] = {}
            
            if x > 0:
                graph[(x, y)][(x - 1, y)] = 1  # Lewo
            if x < width - 1:
                graph[(x, y)][(x + 1, y)] = 1  # Prawo
            if y > 0:
                graph[(x, y)][(x, y - 1)] = 1  # Góra
            if y < height - 1:
                graph[(x, y)][(x, y + 1)] = 1  # Dół

    return graph

def save_graph_to_file(graph, filename):
    """
    Zapisuje graf do pliku za pomocą modułu pickle.
    
    :param graph: Graf do zapisania (słownik).
    :param filename: Nazwa pliku do zapisu.
    """
    with open(filename, 'wb') as f:
        pickle.dump(graph, f)

def load_graph_from_file(filename):
    """
    Wczytuje graf z pliku za pomocą modułu pickle.
    
    :param filename: Nazwa pliku z grafem.
    :return: Wczytany graf (słownik).
    """
    with open(filename, 'rb') as f:
        graph = pickle.load(f)
    return graph

graph_dims = [(200, 50), (200, 200), (500, 500), (1000, 1000), (2000, 2000), (3000, 3000)]

for dim in graph_dims:
    save_graph_to_file(
        create_grid_graph(dim[0], dim[1]),
        f'graph_{dim[0]}_{dim[1]}.pkl'
    )