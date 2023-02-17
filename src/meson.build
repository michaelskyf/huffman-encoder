libhuffman_dep = dependency('libhuffman', fallback: ['libhuffman', 'libhuffman_dep'])
argparse_dep = dependency('argparse', fallback: ['argparse', 'argparse_dep'])
boost_dep = dependency('boost', modules: [ 'json' ])

source_files = files(
	'HuffmanCompressor.cpp',
	'JsonHuffmanParser.cpp',
)

libhuffmanencoder = static_library(
    'huffman',
	source_files,
	dependencies: [ libhuffman_dep, boost_dep ]
)

e = executable(
    'huffman',
	'main.cpp',
	dependencies: [ libhuffman_dep, argparse_dep ],
	link_with: libhuffmanencoder,
    install: true
)
