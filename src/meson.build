libhuffman_dep = dependency('libhuffman', fallback: ['libhuffman', 'libhuffman_dep'])
spdlog_dep = dependency('spdlog', fallback: ['spdlog', 'spdlog_dep'])
argparse_dep = dependency('argparse', fallback: ['argparse', 'argparse_dep'])

source_files = files(
	'HuffmanCompressor.cpp',
	'JsonHuffmanParser.cpp',
)

libhuffmanencoder = static_library(
    'huffman',
	source_files,
	dependencies: [ libhuffman_dep ]
)


e = executable(
    'huffman',
	'main.cpp',
	dependencies: [ libhuffman_dep, spdlog_dep, argparse_dep ],
	link_with: libhuffmanencoder,
    install: true
)
