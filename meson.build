project('ppk-project-huffman', 'cpp',
    version : '1.0',
    license : 'gpl',
    meson_version : '>= 0.61.2',
    default_options : [ 'warning_level=everything', 'buildtype=debugoptimized', 'cpp_std=c++20', 'werror=false' ]
)

cpp_flags = [
  '-pedantic',
]

cpp_warn_blacklist = [
  '-Wno-suggest-attribute=pure', '-Wno-padded', '-Wno-c++98-compat', '-Wno-c++98-compat-pedantic', '-Wno-newline-eof',
  '-Wno-global-constructors'
]

compiler = meson.get_compiler('cpp')
cpp_all_flags = cpp_flags + cpp_warn_blacklist

foreach f :cpp_all_flags
  if compiler.has_argument(f)
    add_global_arguments(f, language : 'cpp')
  endif
endforeach

add_project_arguments('-DVERSION="@0@"'.format(meson.project_version()), language : 'cpp')

library_path = []
project_sources = []
project_header_files = []
project_test_sources = []

subdir('src')

libhuffman_dep = dependency('libhuffman', fallback : ['libhuffman', 'libhuffman_dep'])

ppk_project_huffman_bin = executable('ppk_project_huffman_bin',
  main_source,
  dependencies : libhuffman_dep,
)


subdir('test')
# subdir('docs')
