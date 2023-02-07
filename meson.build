project('ppk-project-huffman', 'cpp',
    version : '1.0',
    license : [ 'gplv3'],
    meson_version : '>= 0.61.2',
    default_options : [ 'warning_level=3', 'buildtype=debugoptimized', 'cpp_std=c++20', 'werror=false' ]
)

add_global_arguments('-Wundef', '-pedantic', language : 'cpp')
add_global_arguments('-Wno-suggest-attribute=pure', '-Wno-padded', language : 'cpp')

add_project_arguments('-DVERSION="@0@"'.format(meson.project_version()), language : 'cpp')

library_path = []
project_sources = []
project_header_files = []
project_test_sources = []

inc = [include_directories('include')]

subdir('src')

ppk_project_huffman_bin_deps = []

ppk_project_huffman_bin_dep_libs = [
  libhuffman
]

ppk_project_huffman_bin = executable('ppk_project_huffman_bin', 
  main_source, 
  include_directories : inc, 
  dependencies : ppk_project_huffman_bin_deps,
  link_with : ppk_project_huffman_bin_dep_libs)


subdir('test')
# subdir('docs')