#ifndef MAZE_MAZE_H
#define MAZE_MAZE_H

#include <list>
#include <queue>
#include <string>
#include <tuple>
#include <vector>

auto const HEIGHT = 10;
auto const WIDTH = 10;

enum class Heuristics {
    Dijkstra,
    Manhattan,
    Euclidean,
};

struct Node {
    int id{};             /// Id of the field.
    int x{};              /// X coordinate of the field.
    int y{};              /// Y coordinate of the field.
    bool start{false};    /// True if the field is the starting point.
    bool end{false};      /// True if the field is the end point.
    bool passable{true};  /// True if one can enter the field.

    /**
     * Prints every value of the field to stdout.
     */
    void debug_print() const;
};

/**
 * Maze data structure implemented with a grid of fields.
 */
class Maze {
   private:
    int w;
    int h;

    /**
     * A helper function for a_star.
     */
    template <typename Heuristic>
    std::pair<std::deque<Node>, std::deque<Node>> _a_star(Heuristic h);

    /**
     * A helper function to print the elapsed time between t1 and t2.
     *
     * @tparam Clock
     * @param t1 Timestamp 1.
     * @param t2 Timestamp 2.
     */
    template <typename Clock>
    void print_elapsed_time(std::chrono::time_point<Clock> t1, std::chrono::time_point<Clock> t2);

   public:
    Node start;
    Node end;

    std::vector<std::vector<Node>> maze;

    /**
     * Returns a maze with random start and end points.
     * There are no guarantees that the end point is reachable in a random maze.
     * There is at most one starting point and one end point.
     *
     * @param random If true there are random obstacles in the maze.
     * @param width Number of columns in the maze.
     * @param height Number of rows in the maze.
     * @param prob Probability of a node being impassable.
     */
    explicit Maze(bool random = false, int width = WIDTH, int height = HEIGHT, int prob = 7);

    /**
     * Delete the old start and set a new one.
     *
     * @param x x-coordinate.
     * @param y y-coordinate.
     */
    void set_start(int x, int y);

    /**
     * Delete the old end and set a new one.
     *
     * @param x x-coordinate.
     * @param y y-coordinate.
     */
    void set_end(int x, int y);

    /**
     * Returns a vector with all neighbots
     *
     * @param node The node whose neighbors we want to get.
     */
    std::vector<Node> get_neighbors(const Node& node);

    /**
     * Makes every node in the maze passable.
     *
     */
    void clear_maze();

    /**
     * Prints the maze in ASCII to stdout.
     */
    void debug_print();

    /**
     * Find the end point in the maze with breadth first search.
     *
     * See also https://en.wikipedia.org/wiki/Breadth-first_search.
     *
     * @return Every node in the order it was visited.
     */
    std::deque<Node> bfs();

    /**
     * Find the end point in the maze with depth first search.
     *
     * See also https://en.wikipedia.org/wiki/Depth-first_search.
     *
     * @return Every node in the order it was visited.
     */
    std::deque<Node> dfs();

    /**
     * Find the shortest path to the end point of the maze with dijkstra's algorithm.
     * The algorithm is implemented with a priority queue.
     *
     * See also https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm.
     *
     * @return Two queues, one containing every visited node and one the shortest path.
     */
    std::pair<std::deque<Node>, std::deque<Node>> dijkstra();

    /**
     * Find the shortest path to the end point of the maze with the A* algorithm.
     *
     * See also https://en.wikipedia.org/wiki/A*_search_algorithm.
     *
     * @param h The heuristics to choose. Possible are Dijkstra (h always returns 0), Manhattan and Euclidean.
     * @return Two queue, one containing every visited node and one the shortest path.
     */
    std::pair<std::deque<Node>, std::deque<Node>> a_star(Heuristics h = Heuristics::Manhattan);
};

#endif  // MAZE_MAZE_H
