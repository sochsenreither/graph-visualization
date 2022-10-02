#include "engine.h"

#include <iostream>

Engine::Engine(bool random, int width, int height) {
    maze = Maze(random, width, height);

    window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "maze");
}

void Engine::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }
}

void Engine::debug_print() {
    maze.debug_print();
}