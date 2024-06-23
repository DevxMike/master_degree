import heapq
import time

def a_star(graph, start, goal, heuristic):
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
            return path
        
        for neighbor in graph[current]:
            tentative_g_score = g_score[current] + graph[current][neighbor]
            if tentative_g_score < g_score[neighbor]:
                came_from[neighbor] = current
                g_score[neighbor] = tentative_g_score
                f_score[neighbor] = g_score[neighbor] + heuristic(neighbor, goal)
                if neighbor not in [i[1] for i in open_list]:
                    heapq.heappush(open_list, (f_score[neighbor], neighbor))
                    
    return None

def dijkstra(graph, start, goal):
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

def create_grid_graph(width, height):
    graph = {}
    
    for y in range(height):
        for x in range(width):
            graph[(x, y)] = {}
            
            if x > 0:
                graph[(x, y)][(x - 1, y)] = 1
            if x < width - 1:
                graph[(x, y)][(x + 1, y)] = 1
            if y > 0:
                graph[(x, y)][(x, y - 1)] = 1
            if y < height - 1:
                graph[(x, y)][(x, y + 1)] = 1
                
    return graph

def heuristic(a, b):
    return abs(a[0] - b[0]) + abs(a[1] - b[1])

class DStar:
    def __init__(self, graph, start, goal, static_algorithm):
        self.graph = graph
        self.start = start
        self.goal = goal
        self.static_algorithm = static_algorithm
        self.open_list = []
        self.g_score = {node: float('inf') for node in graph}
        self.rhs = {node: float('inf') for node in graph}
        self.g_score[goal] = 0
        self.rhs[goal] = 0
        heapq.heappush(self.open_list, (self.calculate_key(goal), goal))

    def calculate_key(self, node):
        return min(self.g_score[node], self.rhs[node])

    def update_vertex(self, node):
        if node != self.goal:
            self.rhs[node] = min(self.g_score[neighbor] + self.graph[node][neighbor] for neighbor in self.graph[node])
        if node in [n[1] for n in self.open_list]:
            self.open_list = [(k, n) for k, n in self.open_list if n != node]
            heapq.heapify(self.open_list)
        if self.g_score[node] != self.rhs[node]:
            heapq.heappush(self.open_list, (self.calculate_key(node), node))

    def compute_shortest_path(self):
        while self.open_list and (self.open_list[0][0] < self.calculate_key(self.start) or self.rhs[self.start] != self.g_score[self.start]):
            _, node = heapq.heappop(self.open_list)
            if self.g_score[node] > self.rhs[node]:
                self.g_score[node] = self.rhs[node]
                for neighbor in self.graph[node]:
                    self.update_vertex(neighbor)
            else:
                self.g_score[node] = float('inf')
                self.update_vertex(node)
                for neighbor in self.graph[node]:
                    self.update_vertex(neighbor)

    def plan(self):
        self.compute_shortest_path()
        path = []
        current = self.start
        while current != self.goal:
            path.append(current)
            min_cost = float('inf')
            next_node = None
            for neighbor in self.graph[current]:
                cost = self.graph[current][neighbor] + self.g_score[neighbor]
                if cost < min_cost:
                    min_cost = cost
                    next_node = neighbor
            if next_node is None:
                return None
            current = next_node
        path.append(self.goal)
        return path

    def update_graph_with_obstacles(self, obstacles):
        for (x, y) in obstacles:
            for dx, dy in [(-1, 0), (1, 0), (0, -1), (0, 1)]:
                neighbor = (x + dx, y + dy)
                if neighbor in self.graph and (x, y) in self.graph[neighbor]:
                    self.graph[neighbor][(x, y)] = float('inf')
        for obstacle in obstacles:
            self.update_vertex(obstacle)

class DStarLite:
    def __init__(self, graph, start, goal, static_algorithm):
        self.graph = graph
        self.start = start
        self.goal = goal
        self.static_algorithm = static_algorithm
        self.open_list = []
        self.g_score = {node: float('inf') for node in graph}
        self.rhs = {node: float('inf') for node in graph}
        self.g_score[start] = float('inf')
        self.rhs[goal] = 0
        heapq.heappush(self.open_list, (self.calculate_key(goal), goal))

    def calculate_key(self, node):
        h = heuristic(self.start, node)
        return (min(self.g_score[node], self.rhs[node]) + h, min(self.g_score[node], self.rhs[node]))

    def update_vertex(self, node):
        if node != self.goal:
            self.rhs[node] = min(self.g_score[neighbor] + self.graph[node][neighbor] for neighbor in self.graph[node])
        if node in [n[1] for n in self.open_list]:
            self.open_list = [(k, n) for k, n in self.open_list if n != node]
            heapq.heapify(self.open_list)
        if self.g_score[node] != self.rhs[node]:
            heapq.heappush(self.open_list, (self.calculate_key(node), node))

    def compute_shortest_path(self):
        while self.open_list and (self.open_list[0][0] < self.calculate_key(self.start) or self.rhs[self.start] != self.g_score[self.start]):
            _, node = heapq.heappop(self.open_list)
            if self.g_score[node] > self.rhs[node]:
                self.g_score[node] = self.rhs[node]
                for neighbor in self.graph[node]:
                    self.update_vertex(neighbor)
            else:
                self.g_score[node] = float('inf')
                self.update_vertex(node)
                for neighbor in self.graph[node]:
                    self.update_vertex(neighbor)

    def plan(self):
        self.compute_shortest_path()
        path = []
        current = self.start
        while current != self.goal:
            path.append(current)
            min_cost = float('inf')
            next_node = None
            for neighbor in self.graph[current]:
                cost = self.graph[current][neighbor] + self.g_score[neighbor]
                if cost < min_cost:
                    min_cost = cost
                    next_node = neighbor
            if next_node is None:
                return None
            current = next_node
        path.append(self.goal)
        return path

    def update_graph_with_obstacles(self, obstacles):
        for (x, y) in obstacles:
            for dx, dy in [(-1, 0), (1, 0), (0, -1), (0, 1)]:
                neighbor = (x + dx, y + dy)
                if neighbor in self.graph and (x, y) in self.graph[neighbor]:
                    self.graph[neighbor][(x, y)] = float('inf')
        for obstacle in obstacles:
            self.update_vertex(obstacle)

def simulate_d_star_lite(graph, start, goal, obstacles, static_algorithm):
    dstar_lite = DStarLite(graph, start, goal, static_algorithm)
    current_position = start
    total_distance = 0
    start_time = time.time()

    for obstacle in obstacles:
        print(f"Dodawanie przeszkody w {obstacle}")
        path = dstar_lite.plan()
        print(f"Ścieżka do przeszkody: {path}")
        
        while path and path[0] != obstacle:
            current_position = path.pop(0)
            total_distance += 1
            print(f"Przemieszczanie się do {current_position}")
        
        dstar_lite.update_graph_with_obstacles([obstacle])
        dstar_lite.start = current_position

    final_path = dstar_lite.plan()
    for pos in final_path:
        total_distance += 1
        print(f"Przemieszczanie się do {pos}")

    end_time = time.time()
    print(f"Ostateczna ścieżka: {final_path}")
    print(f"Czas działania: {end_time - start_time}s, Pokonana odległość: {total_distance}")

def simulate_d_star(graph, start, goal, obstacles, static_algorithm):
    dstar = DStar(graph, start, goal, static_algorithm)
    current_position = start
    total_distance = 0
    start_time = time.time()

    for obstacle in obstacles:
        print(f"Dodawanie przeszkody w {obstacle}")
        path = dstar.plan()
        print(f"Ścieżka do przeszkody: {path}")
        
        while path and path[0] != obstacle:
            current_position = path.pop(0)
            total_distance += 1
            print(f"Przemieszczanie się do {current_position}")
        
        dstar.update_graph_with_obstacles([obstacle])
        dstar.start = current_position

    final_path = dstar.plan()
    for pos in final_path:
        total_distance += 1
        print(f"Przemieszczanie się do {pos}")

    end_time = time.time()
    print(f"Ostateczna ścieżka: {final_path}")
    print(f"Czas działania: {end_time - start_time}s, Pokonana odległość: {total_distance}")

# Przykładowe użycie
width = 200
height = 50
start = (30, 30)
goal = (199, 30)
obstacles = [(30, 30), (60, 30), (90, 30), (120, 30), (150, 30)]

graph = create_grid_graph(width, height)

print("Symulacja D* z A*")
simulate_d_star(graph.copy(), start, goal, obstacles, a_star)

print("\nSymulacja D* z Dijkstrą")
simulate_d_star(graph.copy(), start, goal, obstacles, dijkstra)

print("\nSymulacja D* Lite z A*")
simulate_d_star_lite(graph.copy(), start, goal, obstacles, a_star)

print("\nSymulacja D* Lite z Dijkstrą")
simulate_d_star_lite(graph.copy(), start, goal, obstacles, dijkstra)
