huffman_source = files(
	'HuffmanDictionary.cpp',
	'HuffmanNode.cpp',
	'DecodingByteLoader.cpp',
)

project_sources += huffman_source

libhuffman = static_library('huffman',
	huffman_source,
	include_directories : inc)