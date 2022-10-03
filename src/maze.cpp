#include "maze.h"

#include <cassert>
#include <climits>
#include <iostream>
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

    // Check left
    if (node.x > 0 && maze[node.x - 1][node.y].passable) {
        neighbors.push_back(maze[node.x - 1][node.y]);
    }

    // Check right
    if (node.x < (w - 1) && maze[node.x + 1][node.y].passable) {
        neighbors.push_back(maze[node.x + 1][node.y]);
    }

    // Check up
    if (node.y > 0 && maze[node.x][node.y - 1].passable) {
        neighbors.push_back(maze[node.x][node.y - 1]);
    }

    // Check down
    if (node.y < (h - 1) && maze[node.x][node.y + 1].passable) {
        neighbors.push_back(maze[node.x][node.y + 1]);
    }
    return neighbors;
}

void Maze::clear_maze() {
    for (auto &col: maze) {
        for (auto &n: col) {
            n.passable = true;
        }
    }
}

std::deque<Node> Maze::bfs() {
    std::deque<Node> visited_nodes;

    std::map<int, bool> visited;
    std::list<Node> queue;

    visited[start.id] = true;
    queue.push_back(start);

    while (!queue.empty()) {
        // Get the current node and check if it is the end node.
        auto const node = queue.front();
        queue.pop_front();
        visited_nodes.push_back(node);
        if (node.end) return visited_nodes;

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
    std::deque<Node> visited_nodes;

    std::map<int, bool> visited;
    std::list<Node> stack;

    visited[start.id] = true;
    stack.push_front(start);

    while (!stack.empty()) {
        // Get the current node and check if it is the end node.
        auto const node = stack.front();
        stack.pop_front();
        visited_nodes.push_back(node);
        if (node.end) return visited_nodes;

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
    std::deque<Node> visited_nodes;
    std::deque<Node> shortest_path;
    Node cur;

    std::map<int, int> distance;  // Maps node id -> distance.
    std::map<int, Node> prev;     // Maps node id -> predecessor.

    // Custom comparator. We only care for the first parameter, which is the weight.
    auto comp =
        [](std::pair<int, Node> n1, std::pair<int, Node> n2) {
            return n1.first > n2.first;
        };
    std::priority_queue<std::pair<int, Node>,
                        std::vector<std::pair<int, Node>>,
                        decltype(comp)>
        pq(comp);

    distance[start.id] = 0;
    prev[start.id] = start;
    pq.push(std::pair(0, start));

    while (!pq.empty()) {
        auto const cur_cost = pq.top().first;
        cur = pq.top().second;
        pq.pop();

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
            // If the node doesn't have an entry in distance yet, update it.
            if (distance.find(n.id) == distance.end())
                distance[n.id] = INT_MAX;

            auto const alt = distance[cur.id] + 1;  // The weight is always 1.

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