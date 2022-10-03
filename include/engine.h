#ifndef MAZE_ENGINE_H
#define MAZE_ENGINE_H

#include "SFML/Graphics.hpp"
#include "maze.h"

const auto scale = 20;
const auto border_size = 1;
const auto top_margin = 50;

const auto color_bg = sf::Color(45, 45, 45, 255);
const auto color_rect = sf::Color(225, 225, 225, 255);
const auto color_rect_impassable = sf::Color(65, 65, 65, 255);
const auto color_border = sf::Color(35, 35, 35, 255);
const auto color_start = sf::Color(160, 212, 104, 255);
const auto color_end = sf::Color(72, 207, 173, 255);
const auto color_start_end = sf::Color(252, 110, 81, 255);
const auto color_visited = sf::Color(252, 110, 81, 255);
const auto color_current = sf::Color::Red;
const auto color_shortest_path = sf::Color(216, 51, 74, 255);

class Engine {
   private:
    sf::RenderWindow window;
    sf::Font font;

    std::deque<Node> visited;  /// Contains all nodes that need to be drawn as visited.
    std::deque<Node> sp;       /// Contains nodes of the shortest path.
    int counter_visited{};     /// Index up until visited nodes are drawn.
    int counter_sp{};          /// Index up until sp nodes are drawn.

    int w;  /// Width of the maze.
    int h;  /// Height of the maze.
    int p;  /// Probability for the maze.
    int r;  /// True if the maze has random impassable nodes.

    Maze maze;
    bool finish{false};  /// True if all visited nodes were drawn.
    bool go{false};      /// True if the search algorithm was chosen.


    /**
     * Handles keyboard input.
     * Following key presses are defined:
     * Return: Resets the state of the engine but keeps the same maze.
     * N: Resets the state of the engine and creates a new maze.
     * C: Clears the maze.
     * Esc: Closes the window.
     * Num1: Runs BFS.
     * Num2: Runs DFS.
     * Num3: Runs Dijkstra.
     * Num4: Runs A*.
     *
     *
     */
    void handle_keyboard_input();
    void handle_mouse_input();
    void reset();

    /**
     * Makes every node in the maze passable.
     *
     */
    void clear_maze();

    void draw_ui();
    void draw_rectangle(int x, int y, const sf::Color color);
    void draw_background();
    void draw_visited();
    void draw_shortest_path();
    void draw_current_node();

   public:
    /**
     * Construct a new Engine that is responsible for rendering the maze.
     *
     * @param random If true the maze will have random obstacles.
     */
    Engine(bool random = false, int width = WIDTH, int height = HEIGHT, int prob = 7);

    /**
     * Runs the search algorithm and then draws the progress of the algorithm to sf::window.
     *
     */
    void run();
};

#endif  // MAZE_ENGINE_H