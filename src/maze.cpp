#include "maze.h"

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <map>
#include <random>

void Node::debug_print() const {
    std::cout << "x: "
              << x
              << " y: "
              << y
              << " id: "
              << id
              << (start ? " start " : "")
              << (end ? " end " : "")
              << (passable ? "" : " not passable")
              << std::endl;
}

Maze::Maze(bool random, int width, int height, int prob) {
    assert(width > 0);
    assert(height > 0);

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist_passable(0, prob);
    std::uniform_int_distribution<std::mt19937::result_type> dist_width(0, width - 1);
    std::uniform_int_distribution<std::mt19937::result_type> dist_height(0, height - 1);

    // Randomize start and end points.
    int const start_y = dist_height(rng);
    int const start_x = dist_width(rng);
    int const end_y = dist_height(rng);
    int const end_x = dist_width(rng);

    auto counter = 0;

    for (auto x = 0; x < width; ++x) {
        maze.push_back(std::vector<Node>{});
        for (auto y = 0; y < height; ++y) {
            auto const start = x == start_x && y == start_y;
            auto const end = x == end_x && y == end_y;
            auto const passable = !(random && dist_passable(rng) == 0 && !start && !end);

            maze[x].push_back(Node{
                counter++,
                x,
                y,
                start,
                end,
                passable});
        }
    }

    w = width;
    h = height;
    start = maze[start_x][start_y];
    end = maze[end_x][end_y];
}

void Maze::debug_print() {
    for (auto i = 0; i < w + 2; ++i) {
        std::cout << "//";
    }
    std::cout << "/";
    std::cout << std::endl;

    for (auto y = 0; y < h; ++y) {
        std::cout << "// ";
        for (auto x = 0; x < w; ++x) {
            if (maze[x][y].start)
                std::cout << "s ";
            else if (maze[x][y].end)
                std::cout << "x ";
            else if (maze[x][y].passable)
                std::cout << "_ ";
            else
                std::cout << "o ";
        }
        std::cout << "//";
        std::cout << std::endl;
    }

    for (auto i = 0; i < w + 2; ++i) {
        std::cout << "//";
    }
    std::cout << "/";
    std::cout << std::endl;
}

std::vector<Node> Maze::get_neighbors(const Node& node) {
    std::vector<Node> neighbors;

    auto left = node.x > 0;
    auto right = node.x < (w - 1);
    auto up = node.y > 0;
    auto down = node.y < (h - 1);

    // Check left
    if (left && maze[node.x - 1][node.y].passable)
        neighbors.push_back(maze[node.x - 1][node.y]);

    // Check right
    if (right && maze[node.x + 1][node.y].passable)
        neighbors.push_back(maze[node.x + 1][node.y]);

    // Check up
    if (up && maze[node.x][node.y - 1].passable)
        neighbors.push_back(maze[node.x][node.y - 1]);

    // Check down
    if (down && maze[node.x][node.y + 1].passable)
        neighbors.push_back(maze[node.x][node.y + 1]);

    // /// Diagonal movement
    // // Check top left
    // if (up && left && maze[node.x - 1][node.y - 1].passable)
    //     neighbors.push_back(maze[node.x - 1][node.y - 1]);

    // // Check top right
    // if (up && right && maze[node.x + 1][node.y - 1].passable)
    //     neighbors.push_back(maze[node.x + 1][node.y - 1]);

    // // Check bottom left
    // if (down && left && maze[node.x - 1][node.y + 1].passable)
    //     neighbors.push_back(maze[node.x - 1][node.y + 1]);

    // // Check bottom right
    // if (down && right && maze[node.x + 1][node.y + 1].passable)
    //     neighbors.push_back(maze[node.x + 1][node.y + 1]);

    return neighbors;
}

void Maze::clear_maze() {
    for (auto& col : maze) {
        for (auto& n : col) {
            n.passable = true;
        }
    }
}

std::deque<Node> Maze::bfs() {
    std::deque<Node> visited_nodes;  // List of all nodes visited.

    std::map<int, bool> visited;  // Maps node id -> node already visited.
    std::list<Node> queue;        // List of nodes that need to be processed.

    visited[start.id] = true;
    queue.push_back(start);

    while (!queue.empty()) {
        // Get the current node and check if it is the end node.
        auto const node = queue.front();
        queue.pop_front();
        visited_nodes.push_back(node);
        if (node.end)
            return visited_nodes;

        // Check for unvisited neighbors. If there are any, push them to the back of the queue.
        auto const neighbors = get_neighbors(node);
        for (auto neighbor : neighbors) {
            if (!visited[neighbor.id]) {
                visited[neighbor.id] = true;
                queue.push_back(neighbor);
            }
        }
    }
    return visited_nodes;
}

std::deque<Node> Maze::dfs() {
    std::deque<Node> visited_nodes;  // List of all nodes visited.

    std::map<int, bool> visited;  // Maps node id -> node already visited.
    std::list<Node> stack;        // List of nodes that need to be processed.

    visited[start.id] = true;
    stack.push_front(start);

    while (!stack.empty()) {
        // Get the current node and check if it is the end node.
        auto const node = stack.front();
        stack.pop_front();
        visited_nodes.push_back(node);
        if (node.end)
            return visited_nodes;

        // Check for unvisited neighbors. If there are any, push them to the front of the stack.
        auto const neighbors = get_neighbors(node);
        for (auto neighbor : neighbors) {
            if (!visited[neighbor.id]) {
                visited[neighbor.id] = true;
                stack.push_front(neighbor);
            }
        }
    }
    return visited_nodes;
}

std::pair<std::deque<Node>, std::deque<Node>> Maze::dijkstra() {
    std::deque<Node> visited_nodes;  // List of all nodes visited.
    std::deque<Node> shortest_path;  // List of the nodes which build the shortest path.
    Node cur;

    std::map<int, int> distance;  // Maps node id -> distance.
    std::map<int, Node> prev;     // Maps node id -> predecessor.

    // Custom comparator for min dequeue. We only care for the first parameter, which is the
    // distance.
    auto comp =
        [](std::pair<int, Node> n1, std::pair<int, Node> n2) {
            return n1.first > n2.first;
        };

    // Min priority dequeu, storing pairs <distance, Node>.
    std::priority_queue<std::pair<int, Node>,
                        std::vector<std::pair<int, Node>>,
                        decltype(comp)>
        pq(comp);

    distance[start.id] = 0;
    prev[start.id] = start;
    pq.push(std::pair(0, start));

    while (!pq.empty()) {
        // Get the node of the queue with the shortest distance to the start.
        auto const cur_cost = pq.top().first;
        cur = pq.top().second;
        pq.pop();

        // Push this node to the visited nodes.
        visited_nodes.push_back(cur);

        // Check if the current node is the end.
        if (cur.end)
            break;

        // If we get a node with higher cost continue.
        if (distance[cur.id] < cur_cost)
            continue;

        // Traverse the neighbors of the current node.
        auto neighbors = get_neighbors(cur);
        for (auto const& n : neighbors) {
            // If the node doesn't have an entry in distance yet, update it with infinite distance.
            if (distance.find(n.id) == distance.end())
                distance[n.id] = std::numeric_limits<int>::max();

            // Calculate the distance, which is current distance + 1, since the edge weight is
            // always 1 in our case.
            auto const alt = distance[cur.id] + 1;

            if (alt < distance[n.id]) {
                distance[n.id] = alt;
                prev[n.id] = cur;
                pq.push(std::pair(alt, n));
            }
        }
    }

    // Create shortest path.
    while (true) {
        shortest_path.push_front(cur);
        if (cur.id == prev[cur.id].id)
            break;
        else
            cur = prev[cur.id];
    }
    return std::pair(visited_nodes, shortest_path);
}

template <typename Heuristic>
std::pair<std::deque<Node>, std::deque<Node>> Maze::_a_star(Heuristic h) {
    std::deque<Node> visited_nodes;
    std::deque<Node> shortest_path;

    // Maps n.id -> node preceding n on the cheapest path from start to n.
    std::map<int, Node> came_from;

    // Custom comparator for min dequeue. We only care for the first parameter, which is the
    // distance.
    auto comp =
        [](std::pair<int, Node> n1, std::pair<int, Node> n2) {
            return n1.first > n2.first;
        };

    // Min priority dequeu, storing pairs <f_score, Node>.
    std::priority_queue<std::pair<int, Node>,
                        std::vector<std::pair<int, Node>>,
                        decltype(comp)>
        pq(comp);

    // contains[node.id] is true if node is in pq.
    std::map<int, bool> contains;

    // Maps n.id -> cost of the cheapest path from start to n.
    std::map<int, int> g_score;
    g_score[start.id] = 0;

    // f_sccore[n.id] = g_score[n.id] + h(n).
    std::map<int, int> f_score;
    f_score[start.id] = h(start);

    pq.push(std::pair(f_score[start.id], start));
    contains[start.id] = true;

    while (!pq.empty()) {
        // Get the node with the lowest f_score.
        auto cur = pq.top().second;
        pq.pop();

        // Push this node to the visited nodes.
        visited_nodes.push_back(cur);

        if (cur.end)
            break;

        // If the current node doesn't have a g_score yet, set it to infinity.
        if (g_score.find(cur.id) == g_score.end())
            g_score[cur.id] = std::numeric_limits<int>::max();

        // Traverse the neighbors of the current node.
        auto neighbors = get_neighbors(cur);
        for (auto& n : neighbors) {
            // If n doesn't have an entry in g_score yet, set it to inifinity.
            if (g_score.find(n.id) == g_score.end())
                g_score[n.id] = std::numeric_limits<int>::max();

            // The distance is always 1 in our case.
            auto tentative_g_score = g_score[cur.id] + 1;

            if (tentative_g_score < g_score[n.id]) {
                // We found a better path to the neighbor.
                came_from[n.id] = cur;
                g_score[n.id] = tentative_g_score;
                f_score[n.id] = tentative_g_score + h(n);

                if (!contains[n.id])
                    pq.push(std::pair(f_score[n.id], n));
            }
        }
    }

    // TODO: reconstruct path.
    return std::pair(visited_nodes, shortest_path);
}

std::pair<std::deque<Node>, std::deque<Node>> Maze::a_star(Heuristics heuristic) {
    switch (heuristic) {
        case Heuristics::Dijkstra:
            return _a_star([](Node node) {
                return 0;
            });
        case Heuristics::Manhattan:
            return _a_star([this](Node node) {
                auto dx = std::abs(node.x - end.x);
                auto dy = std::abs(node.y - end.y);
                return dx + dy;
            });
        case Heuristics::Euclidean:
            return _a_star([this](Node node) {
                auto dx = std::abs(node.x - end.x);
                auto dy = std::abs(node.y - end.y);
                return std::sqrt(dx * dx + dy * dy);
            });
    }
}