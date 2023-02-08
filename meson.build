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

inc = [ include_directories('include', get_option('includedir')) ]

subdir('src')

ppk_project_huffman_bin_deps = []

ppk_project_huffman_bin_dep_libs = [
  libhuffman
]

build_exe = get_option('exe')

if not build_exe.disabled()

  ppk_project_huffman_bin = executable('ppk_project_huffman_bin', 
    main_source, 
    include_directories : inc, 
    dependencies : ppk_project_huffman_bin_deps,
    link_with : ppk_project_huffman_bin_dep_libs)

endif


subdir('test')
# subdir('docs')