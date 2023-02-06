huffman_source = files(
	'huffman.cpp',
	'huffman_tree_node.cpp',
	'DecodingByteLoader.cpp',
)

project_sources += huffman_source

libhuffman = static_library('huffman',
	huffman_source,
	include_directories : inc)