src_files = files(
    'maze.cpp',
    'maze_test.cpp'
)

maze_test = executable('maze_test',
    src_files,
    include_directories : incdir,
    dependencies : [catch2_lib]
)
test('maze_test', maze_test)