#include "engine.h"

#include <stdexcept>

#include "fmt/core.h"

Engine::Engine(bool random, int width, int height, int prob) : maze(random, width, height, prob) {
    r = random;
    w = width;
    h = height;
    p = prob;

    auto const window_w = w * (scale + border_size * 2) + 2 * border_size + 2 * scale;
    auto const hindow_h = h * (scale + border_size * 2) + 2 * border_size + 2 * scale + top_margin;

    window.create(sf::VideoMode(window_w, hindow_h), "maze");

    if (!font.loadFromFile("font/SourceSansPro-Regular.otf")) {
        throw std::runtime_error("Failed to open font");
    }
}

void Engine::reset() {
    visited.clear();
    sp.clear();
    counter_visited = 0;
    counter_sp = 0;
    finish = false;
    go = false;
}

void Engine::run() {
    while (window.isOpen()) {
        vertices.clear();
        handle_keyboard_input();
        handle_mouse_input();
        window.clear(color_bg);

        draw_text();
        draw_background_vertex_array();
        draw_visited_vertex_array();
        draw_shorest_path_vertex_array();
        draw_current_node_vertex_array();

        window.draw(&vertices[0], vertices.size(), sf::Quads);
        window.display();
    }
}

/////////////////////////////////////////////////////////////////
/////// Functions below are used for handling user input. ///////
/////////////////////////////////////////////////////////////////

void Engine::handle_mouse_input() {
    // Get current mouse position
    auto mouse_pos = sf::Mouse::getPosition(window);

    auto x_pos = (mouse_pos.x + scale) / (scale + border_size * 2) - 2;
    auto y_pos = (mouse_pos.y - scale - top_margin) / (scale + border_size * 2);

    // Check if the position is inside the 2D vector.
    auto out_of_bound = !(x_pos >= 0 && x_pos < w && y_pos >= 0 && y_pos < h);
    if (out_of_bound)
        return;

    // Check if the position is a starting or end point.
    auto start_or_end = maze.maze[x_pos][y_pos].start || maze.maze[x_pos][y_pos].end;
    if (start_or_end)
        return;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
            maze.set_start(x_pos, y_pos);
        else
            maze.maze[x_pos][y_pos].passable = false;
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
            maze.set_end(x_pos, y_pos);
        else
            maze.maze[x_pos][y_pos].passable = true;
    }
}

void Engine::handle_keyboard_input() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
        reset();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::N)) {
        reset();
        maze = Maze(r, w, h, p);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        window.close();

    if (go)
        return;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
        fmt::print("Selected BFS.. ");
        algorithm = "BFS";
        visited = maze.bfs();
        go = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
        fmt::print("Selected DFS.. ");
        algorithm = "DFS";
        visited = maze.dfs();
        go = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
        fmt::print("Selected Dijkstra.. ");
        algorithm = "Dijkstra";
        auto res = maze.dijkstra();
        visited = res.first;
        sp = res.second;
        go = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
        fmt::print("Selected A* with Manhattan.. ");
        algorithm = "A*";
        auto res = maze.a_star(Heuristics::Manhattan);
        visited = res.first;
        sp = res.second;
        go = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) {
        fmt::print("Selected A* with Euclidean.. ");
        algorithm = "A*";
        auto res = maze.a_star(Heuristics::Euclidean);
        visited = res.first;
        sp = res.second;
        go = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
        maze.clear_maze();
    }
}

/////////////////////////////////////////////////////////////////////////
/////// Functions responsible for drawing vertices to the screen. ///////
/////////////////////////////////////////////////////////////////////////

void Engine::draw_current_node_vertex_array() {
    if (!go || visited.empty())
        return;

    auto cur = visited[counter_visited];
    add_vertices(cur.x, cur.y, color_current);
}

void Engine::draw_shorest_path_vertex_array() {
    if (!finish || sp.empty())
        return;

    if (counter_sp + 1 < sp.size())
        counter_sp++;

    for (unsigned long i = 0; i < counter_sp; ++i) {
        auto const cur = sp[i];
        if (cur.start || cur.end)
            continue;

        add_vertices(cur.x, cur.y, color_shortest_path);
    }
}

void Engine::draw_visited_vertex_array() {
    if (!go || visited.empty())
        return;
    if (counter_visited + 1 < visited.size())
        counter_visited++;
    else
        finish = true;

    for (unsigned long i = 0; i < counter_visited; ++i) {
        auto const cur = visited[i];
        if (cur.start || cur.end)
            continue;

        add_vertices(cur.x, cur.y, color_visited);
    }
}

void Engine::draw_background_vertex_array() {
    for (auto &col : maze.maze) {
        for (auto const &cur : col) {
            sf::Color color;
            if (cur.start && cur.end)
                color = color_start_end;
            else if (cur.start)
                color = color_start;
            else if (cur.end)
                color = color_end;
            else if (cur.passable)
                color = color_rect;
            else
                color = color_rect_impassable;

            add_vertices(cur.x, cur.y, color);
        }
    }
}

void Engine::add_vertices(int x, int y, sf::Color color) {
    auto x_pos = (x * (scale + border_size * 2)) + scale;
    auto y_pos = (y * (scale + border_size * 2)) + scale + top_margin;

    vertices.push_back(sf::Vertex(sf::Vector2f(x_pos, y_pos), color));
    vertices.push_back(sf::Vertex(sf::Vector2f(x_pos, y_pos + scale), color));
    vertices.push_back(sf::Vertex(sf::Vector2f(x_pos + scale, y_pos + scale), color));
    vertices.push_back(sf::Vertex(sf::Vector2f(x_pos + scale, y_pos), color));
}

void Engine::draw_text() {
    sf::Text text;

    text.setFont(font);
    text.setPosition(sf::Vector2f(scale, scale));

    auto s = finish && !sp.empty() ? fmt::format("Steps: {}  Shortest path: {}", counter_visited, counter_sp) : fmt::format("Steps: {}", counter_visited);

    text.setString(s);

    text.setCharacterSize(26);
    text.setFillColor(color_rect);
    text.setStyle(sf::Text::Bold);
    window.draw(text);

    if (!go)
        return;

    text.setString(fmt::format("Running: {}", algorithm));
    auto x = window.getSize().x - text.getLocalBounds().width - scale - 5;

    text.setPosition(sf::Vector2f(x, scale));
    window.draw(text);
}

////////////////////////////////////////////////////////////////////////////
/////// Functions below aren't used anymore for performance reasons. ///////
////////////////////////////////////////////////////////////////////////////

void Engine::draw_rectangle(int x, int y, const sf::Color color) {
    auto x_pos = (-x * (scale + border_size * 2)) - scale;
    auto y_pos = (-y * (scale + border_size * 2)) - scale - top_margin;

    sf::RectangleShape rect(sf::Vector2f(scale, scale));

    // Set color of rectangle.
    rect.setFillColor(color);

    // Set border of rectangle.
    rect.setOutlineThickness(border_size);
    rect.setOutlineColor(color_border);

    // Add an offset so there is a small gap to the window border.
    rect.setOrigin(x_pos, y_pos);
    window.draw(rect);
}

void Engine::draw_background() {
    // TODO: optimize this to vertex array.
    for (auto &col : maze.maze) {
        for (auto const &n : col) {
            auto draw = true;
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

            // Skip the drawing for nodes that will be drawn as visited.
            for (unsigned long i = 0; i < counter_visited; ++i) {
                if (!visited.empty() && n.id == visited[i].id && n.id != maze.start.id)
                    draw = false;
                break;
            }
            if (draw)
                draw_rectangle(n.x, n.y, color);
        }
    }
}

void Engine::draw_visited() {
    if (!go || visited.empty())
        return;
    if (counter_visited + 1 < visited.size())
        counter_visited++;
    else
        finish = true;

    for (unsigned long i = 0; i < counter_visited; ++i) {
        auto const cur = visited[i];
        if (cur.start || cur.end)
            continue;
        draw_rectangle(cur.x, cur.y, color_visited);
    }
}

void Engine::draw_current_node() {
    if (!go || visited.empty())
        return;
    auto cur = visited[counter_visited];
    draw_rectangle(cur.x, cur.y, color_current);
}

void Engine::draw_shortest_path() {
    if (!finish || sp.empty())
        return;

    if (counter_sp + 1 < sp.size())
        counter_sp++;

    for (unsigned long i = 0; i < counter_sp; ++i) {
        auto const cur = sp[i];
        if (cur.start || cur.end)
            continue;
        draw_rectangle(cur.x, cur.y, color_shortest_path);
    }
}