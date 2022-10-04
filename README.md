# Maze visualizer
Maze visualizer is a visualization for different path finding and shortest path algorithms, including:
- `BFS`
- `DFS`
- `Dijkstra`

It is written in `C++17` and is using `SFML` for graphics.

This project is build with [Meson](https://mesonbuild.com/). To build and run it, execute in the root folder:
```
$ meson build
$ ninja -C build graph-visualization
$ ./build/graph-visualization
```

## Controls
The size of the maze can be configured in `main.cpp` when constructing the `engine` object. After starting, following inputs are possible:
- `Left click` on a node to make it impassable
- `Right click` on a node to make it passable
- `Enter` to reset the program state
- `Esc` to close the window
- `n` to generate a new maze
- `c` to remove every impassable node from the maze
- `1` to run DFS
- `2` to run BFS
- `3` to run Dijkstra
- `4` to run A*

## External libraries used by this project

These dependencies will be downloaded automatically when building the project.

- [SFML](https://github.com/SFML/SFML)
- [Catch2](https://github.com/catchorg/Catch2)
- [fmt](https://github.com/fmtlib/fmt)

## To-do
- Implement A*
- Implement maze generation algorithms