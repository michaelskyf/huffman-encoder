test_sources = [
    'ByteLoader.cpp',
	'ByteDecoder.cpp'
]

e = executable('decoder', test_sources,
		dependencies : gtest_main_dep,
		include_directories : [inc],
		link_with : [libhuffman])

test('decoder', e)