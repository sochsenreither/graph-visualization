project('graph-visualization', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++17'])

sfml_dep = dependency('sfml')
catch2_dep = dependency('catch2')
fmt_dep = dependency('fmt')

incdir = include_directories('include')

subdir('src')
subdir('test')

executable('graph-visualization',
    'main.cpp',
    maze_files,
    engine_files,
    include_directories : incdir,
    dependencies: [sfml_dep, fmt_dep]
)
