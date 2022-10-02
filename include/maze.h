#ifndef MAZE_MAZE_H
#define MAZE_MAZE_H

#include <tuple>
#include <vector>

auto const HEIGHT = 10;
auto const WIDTH = 10;

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

   public:
    Node start;  /// Starting point of the maze.
    std::vector<std::vector<Node>> maze; 

    /**
     * Returns a vector with all neighbots
     *
     * @param node The node whose neighbors we want to get.
     */
    std::vector<Node> get_neighbors(const Node& node);

    /**
     * Returns a maze with random start and end points.
     * There are no guarantees that the end point is reachable in a random maze.
     * There is at most one starting point and one end point.
     *
     * @param random If true there are random obstacles in the maze.
     * @param width Number of columns in the maze.
     * @param height Number of rows in the maze.
     * @param prob Probability of a node being unpassable.
     */
    explicit Maze(bool random = false, int width = WIDTH, int height = HEIGHT, int prob = 7);

    /**
     * Prints the maze in ASCII to stdout.
     */
    void debug_print();

    /**
     * Find the end point in the maze with breadth first search.
     *
     * See also https://en.wikipedia.org/wiki/Breadth-first_search.
     *
     * @param s The starting point for the algorithm.
     * @return Every field in the order it was visited.
     */
    std::vector<Node> bfs(const Node& s);

    /**
     * Find the end point in the maze with depth first search.
     *
     * See also https://en.wikipedia.org/wiki/Depth-first_search.
     *
     * @param s The starting point for the algorithm.
     * @return Every field in the order it was visited.
     */
    std::vector<Node> dfs(const Node& s);

    /**
     * Find the shortest path to the end point of the maze with dijkstra's algorithm.
     *
     * See also https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm.
     *
     * @param s The starting point for the algorithm.
     * @return Two vectors, one containing every visited node and one the shortest path.
     */
    std::tuple<std::vector<Node>, std::vector<Node>> dijkstra(const Node& s);

    /**
     * Find the shortest path to the end point of the maze with the Bellman-Ford algorithm.
     *
     * See also https://en.wikipedia.org/wiki/Bellman%E2%80%93Ford_algorithm.
     *
     * @param s The starting point for the algorithm.
     * @return Two vectors, one containing every visited node and one the shortest path.
     */
    std::tuple<std::vector<Node>, std::vector<Node>> bellman_ford(const Node& s);

    /**
     * Find the shortest path to the end point of the maze with the A* algorithm.
     *
     * See also https://en.wikipedia.org/wiki/A*_search_algorithm.
     *
     * @param s The starting point for the algorithm.
     * @return Two vectors, one containing every visited node and one the shortest path.
     */
    std::tuple<std::vector<Node>, std::vector<Node>> a_star(const Node& s);
};

#endif  // MAZE_MAZE_H
