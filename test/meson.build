test_files = files(
    'maze_test.cpp'
)

maze_test = executable('maze_test',
    test_files,
    maze_files,
    include_directories : incdir,
    dependencies : [catch2_dep]
)
test('maze_test', maze_test)