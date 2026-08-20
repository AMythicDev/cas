project('cas', 'cpp', version: '0.1.0', 
  default_options: ['cpp_std=c++23', 'warning_level=3']
)

if meson.get_compiler('cpp').get_id() != 'msvc'
  add_project_arguments('-pedantic-errors', language : 'cpp')
else
  add_project_arguments('/permissive-', language : 'cpp')
endif

cas_inc = include_directories('include/')
cas = static_library('cas', 'src/simulator.cpp', include_directories: [cas_inc])

cas_dep = declare_dependency(link_with : cas,
  include_directories : cas_inc)

subdir('tests/')
subdir('examples/')

