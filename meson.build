project('huffman-encoder', 'cpp',
    version : '1.0',
    license : 'gpl',
    meson_version : '>= 0.61.2',
    default_options : [ 'warning_level=everything', 'buildtype=debug', 'cpp_std=c++20', 'werror=false' ]
)

cpp_flags = [

]

cpp_warn_blacklist = [
  '-Wno-suggest-attribute=pure', '-Wno-suggest-attribute=const', '-Wno-padded', '-Wno-c++98-compat', '-Wno-c++98-compat-pedantic',
  '-Wno-global-constructors', '-Wno-newline-eof', '-Wno-strict-overflow', '-Wno-effc++', '-Wno-unused-const-variable',
  '-Wno-abi-tag', '-Wno-switch-default', '-Wno-inline'
]

compiler = meson.get_compiler('cpp')
cpp_all_flags = cpp_flags + cpp_warn_blacklist

foreach f :cpp_all_flags
  if compiler.has_argument(f)
    add_project_arguments(f, language : 'cpp')
  endif
endforeach

add_project_arguments('-DVERSION="@0@"'.format(meson.project_version()), language : 'cpp')

subdir('src')
subdir('test')
# subdir('docs')
