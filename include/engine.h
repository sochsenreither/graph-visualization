#ifndef MAZE_ENGINE_H
#define MAZE_ENGINE_H

#include "SFML/Graphics.hpp"
#include "maze.h"

const auto scale = 20;
const auto border_size = 1;

const auto color_bg = sf::Color(45, 45, 45, 255);
const auto color_rect = sf::Color(225, 225, 225, 255);
const auto color_rect_unpassable = sf::Color(65, 65, 65, 255);
const auto color_border = sf::Color(35, 35, 35, 255);
const auto color_start = sf::Color(160, 212, 104, 255);
const auto color_end = sf::Color(72, 207, 173, 255);
const auto color_start_end = sf::Color(252, 110, 81, 255);
const auto color_visited = sf::Color(252, 110, 81, 255);
const auto color_current = sf::Color(216, 51, 74, 255);
const auto color_shortest_path = sf::Color::Red;

class Engine {
   private:
    sf::RenderWindow window;
    std::list<Node> visited;  /// Contains visited nodes by the search algorithm.
    Maze maze;
    bool finish{false};

    void draw_rectangle(int x, int y, const sf::Color color);
    void draw_background();
    void draw_visited();
    void draw_shortest_path();

   public:
    /**
     * Construct a new Engine that is responsible for rendering the maze.
     *
     * @param random If true the maze will have random obstacles.
     */
    Engine(bool random = false, int width = WIDTH, int height = HEIGHT);

    void run();
    void debug_print();
};

#endif  // MAZE_ENGINE_H