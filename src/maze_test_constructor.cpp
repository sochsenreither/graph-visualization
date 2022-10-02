/**
 * This file just contains a constructor for deterministic mazes for testing purposes.
 */
#include "maze.h"

Maze::Maze(int id) {
    maze = std::array<std::array<Node, 4>, 4>{};
    auto counter = 0;
    if (id == 0) {
        // Maze:
        /////////////
        // s x _ _ //
        // _ _ _ _ //
        // _ _ _ _ //
        // _ _ _ _ //
        /////////////
        for (auto x = 0; x < WIDTH; ++x) {
            for (auto y = 0; y < HEIGHT; ++y) {
                maze[x][y].x = x;
                maze[x][y].y = y;
                maze[x][y].id = counter++;
            }
        }
        maze[0][0].start = true;
        maze[1][0].end = true;
        start = maze[0][0];
    }

    if (id == 1) {
        // Maze:
        /////////////
        // s o x o //
        // _ o _ _ //
        // _ _ o _ //
        // o _ _ _ //
        /////////////
        for (auto x = 0; x < WIDTH; ++x) {
            for (auto y = 0; y < HEIGHT; ++y) {
                maze[x][y].x = x;
                maze[x][y].y = y;
                maze[x][y].id = counter++;
            }
        }
        maze[0][0].start = true;
        maze[2][0].end = true;
        start = maze[0][0];
        maze[1][0].passable = false;
        maze[3][0].passable = false;
        maze[1][1].passable = false;
        maze[2][2].passable = false;
        maze[0][3].passable = false;
    }

    if (id == 2) {
        // Maze:
        /////////////
        // _ _ _ _ //
        // _ s _ _ //
        // _ _ x _ //
        // _ _ _ _ //
        /////////////
        for (auto x = 0; x < WIDTH; ++x) {
            for (auto y = 0; y < HEIGHT; ++y) {
                maze[x][y].x = x;
                maze[x][y].y = y;
                maze[x][y].id = counter++;
            }
        }
        maze[1][1].start = true;
        maze[2][2].end = true;
        start = maze[1][1];
    }

    if (id == 3) {
        // Maze:
        /////////////
        // s x _ _ //
        // o _ _ _ //
        // _ _ _ _ //
        // _ _ _ _ //
        /////////////
        for (auto x = 0; x < WIDTH; ++x) {
            for (auto y = 0; y < HEIGHT; ++y) {
                maze[x][y].x = x;
                maze[x][y].y = y;
                maze[x][y].id = counter++;
            }
        }
        maze[0][0].start = true;
        maze[1][0].end = true;
        maze[0][1].passable = false;
        start = maze[0][0];
    }

    if (id == 4) {
        // Maze:
        /////////////
        // _ _ x _ //
        // _ o s o //
        // _ _ _ _ //
        // _ _ _ _ //
        /////////////
        for (auto x = 0; x < WIDTH; ++x) {
            for (auto y = 0; y < HEIGHT; ++y) {
                maze[x][y].x = x;
                maze[x][y].y = y;
                maze[x][y].id = counter++;
            }
        }
        maze[2][1].start = true;
        maze[2][0].end = true;
        maze[1][1].passable = false;
        maze[3][1].passable = false;
        start = maze[2][1];
    }
}