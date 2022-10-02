#define CATCH_CONFIG_MAIN

#include "maze.h"

#include "catch2/catch.hpp"

/**
 * Create a test maze.
 *
 * @return An empty 4x4 maze.
 */
Maze test_maze() {
    auto maze = Maze(false, 4, 4);

    for (auto &col : maze.maze) {
        for (auto &n : col) {
            n.start = false;
            n.end = false;
        }
    }
    return maze;
}

TEST_CASE("Maze with obstacles", "[maze]") {
    /////////////
    // s o x o //
    // _ o _ _ //
    // _ _ o _ //
    // o _ _ _ //
    /////////////
    auto maze = test_maze();

    maze.maze[0][0].start = true;
    maze.maze[2][0].end = true;
    maze.start = maze.maze[0][0];
    maze.maze[1][0].passable = false;
    maze.maze[3][0].passable = false;
    maze.maze[1][1].passable = false;
    maze.maze[2][2].passable = false;
    maze.maze[0][3].passable = false;

    auto const neighbors = maze.get_neighbors(maze.start);

    REQUIRE(neighbors.size() == 1);
}

TEST_CASE("Maze without ostacles", "[maze]") {
    /////////////
    // _ _ _ _ //
    // _ s _ _ //
    // _ _ x _ //
    // _ _ _ _ //
    /////////////
    auto maze = test_maze();

    maze.maze[1][1].start = true;
    maze.maze[2][2].end = true;
    maze.start = maze.maze[1][1];

    auto const neighbors = maze.get_neighbors(maze.start);

    REQUIRE(neighbors.size() == 4);
}

TEST_CASE("Maze with end next to start", "[maze]") {
    /////////////
    // s x _ _ //
    // o _ _ _ //
    // _ _ _ _ //
    // _ _ _ _ //
    /////////////
    auto maze = test_maze();

    maze.maze[0][0].start = true;
    maze.maze[1][0].end = true;
    maze.maze[0][1].passable = false;
    maze.start = maze.maze[0][0];

    SECTION("dfs") {
        auto const dfs_res = maze.dfs();
        REQUIRE(dfs_res.size() == 2);
    }

    SECTION("dfs") {
        auto const bfs_res = maze.bfs();
        REQUIRE(bfs_res.size() == 2);
    }
}

TEST_CASE("BFS and DFS test", "[maze]") {
    /////////////
    // _ _ x _ //
    // _ o s o //
    // _ _ _ _ //
    // _ _ _ _ //
    /////////////
    auto maze = test_maze();

    maze.maze[2][1].start = true;
    maze.maze[2][0].end = true;
    maze.maze[1][1].passable = false;
    maze.maze[3][1].passable = false;
    maze.start = maze.maze[2][1];

    SECTION("dfs") {
        auto const dfs_res = maze.dfs();
        REQUIRE(dfs_res.size() > 3);
    }

    SECTION("dfs") {
        auto const bfs_res = maze.bfs();
        REQUIRE(bfs_res.size() == 2);
    }
}