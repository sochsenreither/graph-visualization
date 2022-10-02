#include "engine.h"

#include <iostream>

Engine::Engine(bool random, int width, int height, int prob) {
    maze = Maze(random, width, height, prob);

    auto const w = width * (scale + border_size * 2) + 2 * border_size + 2 * scale;
    auto const h = height * (scale + border_size * 2) + 2 * border_size + 2 * scale;

    window.create(sf::VideoMode(w, h), "maze");
}

void Engine::run() {
    auto res = maze.dijkstra();
    auto nodes = res.first;
    auto shortest_path = res.second;
    Node cur;

    while (window.isOpen()) {
        // Check for window events.
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Clear the window with grey color.
        window.clear(color_bg);

        // Draw the background of the maze.
        draw_background();

        // Draw the visited nodes of the search algorithm.
        draw_visited();

        // Draw the shortest path
        if (finish && !shortest_path.empty()) {
            sp.push_back(shortest_path.front());
            shortest_path.pop_front();
        }
        draw_shortest_path();

        // Draw the current node.
        if (!finish) {
            cur = nodes.front();
            nodes.pop_front();
            visited.push_back(cur);
            if (nodes.empty())
                finish = true;
        }

        auto x = -cur.x * (scale + border_size * 2);
        auto y = -cur.y * (scale + border_size * 2);
        draw_rectangle(x, y, color_current);

        // End the current frame.
        window.display();
    }
}

void Engine::draw_shortest_path() {
    for (auto &n : sp) {
        if (n.start || n.end) {
            continue;
        }
        auto x = -n.x * (scale + border_size * 2);
        auto y = -n.y * (scale + border_size * 2);
        draw_rectangle(x, y, color_shortest_path);
    }
}

void Engine::draw_visited() {
    for (auto &n : visited) {
        if (n.start || n.end) {
            continue;
        }
        auto x = -n.x * (scale + border_size * 2);
        auto y = -n.y * (scale + border_size * 2);
        draw_rectangle(x, y, color_visited);
    }
}

void Engine::draw_background() {
    // TODO: optimize this to vertex array.
    for (auto &col : maze.maze) {
        for (auto &n : col) {
            auto x = -n.x * (scale + border_size * 2);
            auto y = -n.y * (scale + border_size * 2);
            sf::Color color;
            if (n.start && n.end)
                color = color_start_end;
            else if (n.start)
                color = color_start;
            else if (n.end)
                color = color_end;
            else if (n.passable)
                color = color_rect;
            else
                color = color_rect_impassable;
            draw_rectangle(x, y, color);
        }
    }
}

void Engine::draw_rectangle(int x, int y, const sf::Color color) {
    sf::RectangleShape rect(sf::Vector2f(scale, scale));

    // Set color of rectangle.
    rect.setFillColor(color);

    // Set border of rectangle.
    rect.setOutlineThickness(border_size);
    rect.setOutlineColor(color_border);

    // Add an offset so there is a small gap to the window border.
    rect.setOrigin(x - scale, y - scale);
    window.draw(rect);
}

void Engine::debug_print() {
    maze.debug_print();
}
