test_sources = [
    'huffman.cpp',
	'DecodingByteLoader.cpp'
]

e = executable('test_huffman', test_sources,
		dependencies : gtest_main_dep,
		include_directories : [inc],
		link_with : [libhuffman])

test('huffman', e)