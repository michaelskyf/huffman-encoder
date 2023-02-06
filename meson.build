project('ppk-project-huffman', 'cpp',
    version : '1.0',
    license : [ 'proprietary'],
    meson_version : '>= 0.61.2',
    default_options : [ 'warning_level=3', 'buildtype=debugoptimized', 'cpp_std=c++20' ]
)

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