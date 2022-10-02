#define CATCH_CONFIG_MAIN

#include "maze.h"

#include <catch2/catch.hpp>

TEST_CASE("Maze with obstacles", "[maze]") {
    /////////////
    // s o x o //
    // _ o _ _ //
    // _ _ o _ //
    // o _ _ _ //
    /////////////
    auto maze = Maze(1);
    auto neighbors = maze.get_neighbors(maze.start);

    REQUIRE(neighbors.size() == 1);
}

TEST_CASE("Maze without ostacles", "[maze]") {
    /////////////
    // _ _ _ _ //
    // _ s _ _ //
    // _ _ x _ //
    // _ _ _ _ //
    /////////////
    auto maze = Maze(2);
    auto neighbors = maze.get_neighbors(maze.start);

    REQUIRE(neighbors.size() == 4);
}

TEST_CASE("Maze with end next to start", "[maze]") {
    /////////////
    // s x _ _ //
    // o _ _ _ //
    // _ _ _ _ //
    // _ _ _ _ //
    /////////////
    auto maze = Maze(3);

    SECTION("dfs") {
        auto dfs_res = maze.dfs(maze.start);
        REQUIRE(dfs_res.size() == 2);
    }

    SECTION("dfs") {
        auto bfs_res = maze.bfs(maze.start);
        REQUIRE(bfs_res.size() == 2);
    }
}

TEST_CASE("BFS and DFS test", "[maze]") {
    /////////////
    // _ _ _ _ //
    // _ o s o //
    // _ _ x _ //
    // _ _ _ _ //
    /////////////
    auto maze = Maze(4);

    SECTION("dfs") {
        auto dfs_res = maze.dfs(maze.start);
        REQUIRE(dfs_res.size() > 3);
    }

    SECTION("dfs") {
        auto bfs_res = maze.bfs(maze.start);
        REQUIRE(bfs_res.size() == 2);
    }
}