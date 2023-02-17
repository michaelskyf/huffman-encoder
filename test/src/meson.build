test_sources = [
    'HuffmanCompressor.cpp',
]

e = executable('huffman', test_sources,
		dependencies : [ gtest_main_dep, libhuffman_dep ],
		include_directories : [inc],
		link_with : [libhuffmanencoder])

test('huffman', e)