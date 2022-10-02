#ifndef MAZE_ENGINE_H
#define MAZE_ENGINE_H

#include "SFML/Window.hpp"
#include "maze.h"

const auto WINDOW_WIDTH = 1280;
const auto WINDOW_HEIGHT = 900;

class Engine {
   private:
    sf::Window window;

   public:
    Maze maze;
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