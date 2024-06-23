import heapq
import math
import time
import matplotlib.pyplot as plt

class Node:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.g = math.inf
        self.rhs = math.inf
        self.key = (self.g, self.rhs)
        self.parent = None
    
    def __lt__(self, other):
        return self.key < other.key
    
    def __eq__(self, other):
        if isinstance(other, Node):
            return self.x == other.x and self.y == other.y
        return False
    
    def __hash__(self):
        return hash((self.x, self.y))
    
    def update_key(self):
        self.key = (self.g, self.rhs)


class DStarLite:
    def __init__(self, graph, start, goal, heuristic):
        self.graph = graph
        self.start = start
        self.goal = goal
        self.heuristic = heuristic
        self.km = 0  # km value for maintaining priority queue
        self.open_list = []
        self.U = {}
        self.rhs = {}
        self.initialize()
    
    def initialize(self):
        self.rhs[self.goal] = 0
        heapq.heappush(self.open_list, (self.calculate_key(self.goal), self.goal))
    
    def calculate_key(self, node):
        min_g_rhs = min(self.g(node), self.rhs.get(node, math.inf))
        return (min_g_rhs + self.heuristic(node, self.start) + self.km, min_g_rhs)
    
    def g(self, node):
        return self.U.get(node, math.inf)
    
    def update_vertex(self, node):
        if node != self.goal:
            self.rhs[node] = min([self.graph.cost(node, succ) + self.g(succ) for succ in self.graph.neighbors(node)])
        if node in self.open_list:
            self.open_list.remove((self.calculate_key(node), node))
        if self.g(node) != self.rhs.get(node, math.inf):
            heapq.heappush(self.open_list, (self.calculate_key(node), node))
    
    def compute_shortest_path(self):
        while self.open_list and (self.open_list[0][0] < self.calculate_key(self.start) or self.rhs.get(self.start, math.inf) != self.g(self.start)):
            k_old, u = heapq.heappop(self.open_list)
            if k_old < self.calculate_key(u):
                heapq.heappush(self.open_list, (self.calculate_key(u), u))
            elif self.g(u) > self.rhs.get(u, math.inf):
                self.U[u] = self.rhs[u]
                for pred in self.graph.predecessors(u):
                    self.update_vertex(pred)
            else:
                self.U[u] = math.inf
                for pred in self.graph.predecessors(u):
                    self.update_vertex(pred)
                self.update_vertex(u)
    
    def replan(self):
        self.km += self.heuristic(self.start, self.goal)
        self.compute_shortest_path()
    
    def update_graph_with_obstacles(self, obstacles):
        self.graph.add_obstacles(obstacles)
    
    def plan(self):
        self.compute_shortest_path()
        return self.extract_path()
    
    def extract_path(self):
        path = [self.start]
        while path[-1] != self.goal:
            current_node = path[-1]
            successors = self.graph.neighbors(current_node)
            min_cost = math.inf
            next_node = None
            for succ in successors:
                cost = self.graph.cost(current_node, succ)
                if cost + self.g(succ) < min_cost:
                    min_cost = cost + self.g(succ)
                    next_node = succ
            if next_node:
                path.append(next_node)
        return path

class DStar:
    def __init__(self, graph, start, goal, heuristic):
        self.graph = graph
        self.start = start
        self.goal = goal
        self.heuristic = heuristic
        self.km = 0  # km value for maintaining priority queue
        self.open_list = []
        self.U = {}
        self.rhs = {}
        self.initialize()
    
    def initialize(self):
        self.rhs[self.goal] = 0
        heapq.heappush(self.open_list, (self.calculate_key(self.goal), self.goal))
    
    def calculate_key(self, node):
        min_g_rhs = min(self.g(node), self.rhs.get(node, math.inf))
        return (min_g_rhs + self.heuristic(node, self.start) + self.km, min_g_rhs)
    
    def g(self, node):
        return self.U.get(node, math.inf)
    
    def update_vertex(self, node):
        if node != self.goal:
            self.rhs[node] = min([self.graph.cost(node, succ) + self.g(succ) for succ in self.graph.neighbors(node)])
        if node in self.open_list:
            self.open_list.remove((self.calculate_key(node), node))
        if self.g(node) != self.rhs.get(node, math.inf):
            heapq.heappush(self.open_list, (self.calculate_key(node), node))
    
    def compute_shortest_path(self):
        while self.open_list and (self.open_list[0][0] < self.calculate_key(self.start) or self.rhs.get(self.start, math.inf) != self.g(self.start)):
            k_old, u = heapq.heappop(self.open_list)
            if k_old < self.calculate_key(u):
                heapq.heappush(self.open_list, (self.calculate_key(u), u))
            elif self.g(u) > self.rhs.get(u, math.inf):
                self.U[u] = self.rhs[u]
                for pred in self.graph.predecessors(u):
                    self.update_vertex(pred)
            else:
                self.U[u] = math.inf
                for pred in self.graph.predecessors(u):
                    self.update_vertex(pred)
                self.update_vertex(u)
    
    def replan(self):
        self.km += self.heuristic(self.start, self.goal)
        self.compute_shortest_path()
    
    def update_graph_with_obstacles(self, obstacles):
        self.graph.add_obstacles(obstacles)
    
    def plan(self):
        self.compute_shortest_path()
        return self.extract_path()
    
    def extract_path(self):
        path = [self.start]
        while path[-1] != self.goal:
            current_node = path[-1]
            successors = self.graph.neighbors(current_node)
            min_cost = math.inf
            next_node = None
            for succ in successors:
                cost = self.graph.cost(current_node, succ)
                if cost + self.g(succ) < min_cost:
                    min_cost = cost + self.g(succ)
                    next_node = succ
            if next_node:
                path.append(next_node)
        return path


class Graph:
    def __init__(self):
        self.obstacles = set()
    
    def add_obstacles(self, obstacles):
        self.obstacles.update(obstacles)
    
    def cost(self, node1, node2):
        if (node1.x, node1.y) in self.obstacles or (node2.x, node2.y) in self.obstacles:
            return math.inf  # assuming infinite cost for obstacles
        return math.sqrt((node1.x - node2.x) ** 2 + (node1.y - node2.y) ** 2)
    
    def neighbors(self, node):
        neighbors = []
        for dx in [-1, 0, 1]:
            for dy in [-1, 0, 1]:
                if dx == 0 and dy == 0:
                    continue
                neighbor = Node(node.x + dx, node.y + dy)
                if (neighbor.x, neighbor.y) not in self.obstacles:
                    neighbors.append(neighbor)
        return neighbors
    
    def predecessors(self, node):
        return self.neighbors(node)


# Funkcja do rysowania mapy z trasą
def draw_map(graph, start, goal, obstacles, path_x, path_y):
    plt.figure(figsize=(10, 8))
    
    # Rysowanie przeszkód
    for obstacle in obstacles:
        plt.scatter(obstacle[0], obstacle[1], color='red', s=100, marker='s')
    
    # Rysowanie startu i celu
    plt.scatter(start.x, start.y, color='green', s=100, marker='o', label='Start')
    plt.scatter(goal.x, goal.y, color='blue', s=100, marker='o', label='Cel')
    
    # Rysowanie ścieżki
    if path_x:
        plt.plot(path_x, path_y, linestyle='-', linewidth=2, color='black', label='Ścieżka')
    
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.title('Mapa z trasą robotu')
    plt.legend()
    plt.grid(True)
    plt.show()



# Test symulacji

def simulate_d_star(graph, start, goal, obstacles):
    heuristic = lambda n1, n2: abs(n1.x - n2.x) + abs(n1.y - n2.y)  # Manhattan distance heuristic
    dstar = DStar(graph, start, goal, heuristic)
    current_position = start
    total_distance = 0
    start_time = time.time()
    px = []
    py = []

    for obstacle in obstacles:
        # print(f"Dodawanie przeszkody w {obstacle}")
        
        # Aktualizacja grafu o nową przeszkodę i replanowanie
        dstar.update_graph_with_obstacles([obstacle])
        dstar.start = current_position
        
        # Planowanie ścieżki
        path = dstar.plan()
        
        if path:
            # Przemieszczanie się do miejsca przeszkody lub celu
            for pos in path:
                current_position = pos
                total_distance += 1
                px.append(pos.x)
                py.append(pos.y)
                # print(f"Przemieszczanie się do {current_position.x}, {current_position.y}")
        
        else:
            print(f"Nie udało się znaleźć ścieżki do celu po dodaniu przeszkody {obstacle}.")
            break
    
    # Planowanie ostatecznej ścieżki do celu
    final_path = dstar.plan()
    for pos in final_path:
        total_distance += 1
        px.append(pos.x)
        py.append(pos.y)
        # print(f"Przemieszczanie się do {pos.x}, {pos.y}")

    end_time = time.time()
    # print(f"Ostateczna ścieżka: {final_path}")
    print(f"Czas działania dstar: {end_time - start_time}s, Pokonana odległość: {total_distance}")
    
    # Rysowanie mapy z trasą
    # draw_map(graph, start, goal, obstacles, px, py)

def simulate_d_star_lite(graph, start, goal, obstacles):
    heuristic = lambda n1, n2: abs(n1.x - n2.x) + abs(n1.y - n2.y)  # Manhattan distance heuristic
    dstar_lite = DStarLite(graph, start, goal, heuristic)
    current_position = start
    total_distance = 0
    start_time = time.time()

    for obstacle in obstacles:
        # print(f"Dodawanie przeszkody w {obstacle}")
        
        # Aktualizacja grafu o nową przeszkodę i replanowanie
        dstar_lite.update_graph_with_obstacles([obstacle])
        dstar_lite.start = current_position
        
        # Planowanie ścieżki
        path = dstar_lite.plan()
        
        if path:
            # Przemieszczanie się do miejsca przeszkody lub celu
            for pos in path:
                current_position = pos
                total_distance += 1
                # print(f"Przemieszczanie się do {current_position.x}")
        
        else:
            print(f"Nie udało się znaleźć ścieżki do celu po dodaniu przeszkody {obstacle}.")
            break
    
    # Planowanie ostatecznej ścieżki do celu
    px = []
    py = []
    final_path = dstar_lite.plan()
    for pos in final_path:
        total_distance += 1
        # print(f"Przemieszczanie się do {pos}")
        px.append(pos.x)
        py.append(pos.y)

    end_time = time.time()
    # print(f"Ostateczna ścieżka: {final_path}")
    print(f"Czas działania dstar_lite: {end_time - start_time}s, Pokonana odległość: {total_distance}")
    # print(final_path)
    # Rysowanie mapy z trasą
    # print(px, py)
    # draw_map(graph, start, goal, obstacles, px, py)


# Przykładowe użycie

print("Scenariusz 1")

graph = Graph()
start_node = Node(0, 0)
goal_node = Node(100, 100)
obstacles = [
    (30, 30), (60, 30), (20, 50), (70, 20), (50, 70),
    (80, 40), (10, 80), (90, 90), (40, 10), (5, 25)
]

simulate_d_star(graph, start_node, goal_node, obstacles)
simulate_d_star_lite(graph, start_node, goal_node, obstacles)

print("Scenariusz 2")

graph = Graph()
start_node = Node(0, 0)
goal_node = Node(100, 100)
obstacles = [
    (20, 30), (40, 50), (60, 70), (80, 90)
]

simulate_d_star(graph, start_node, goal_node, obstacles)
simulate_d_star_lite(graph, start_node, goal_node, obstacles)

print("Scenariusz 3")

graph = Graph()
start_node = Node(0, 0)
goal_node = Node(100, 100)
obstacles = []

simulate_d_star(graph, start_node, goal_node, obstacles)
simulate_d_star_lite(graph, start_node, goal_node, obstacles)

print("Scenariusz 4")

graph = Graph()
start_node = Node(0, 0)
goal_node = Node(100, 100)
obstacles = [
    (50, 50), (50, 51), (51, 50), (51, 51)
]

simulate_d_star(graph, start_node, goal_node, obstacles)
simulate_d_star_lite(graph, start_node, goal_node, obstacles)

print("Scenariusz 5")

graph = Graph()
start_node = Node(0, 0)
goal_node = Node(100, 100)
obstacles = [
    (30, 30), (60, 30), (20, 50), (70, 20), (50, 70),
    (80, 40), (10, 80), (90, 90), (40, 10), (5, 25),
    (40, 40), (60, 60), (80, 80)
]

simulate_d_star(graph, start_node, goal_node, obstacles)
simulate_d_star_lite(graph, start_node, goal_node, obstacles)