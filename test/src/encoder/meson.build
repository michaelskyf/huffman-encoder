test_sources = [
    'ByteWriter.cpp',
	'ByteEncoder.cpp'
]

e = executable('encoder', test_sources,
		dependencies : gtest_main_dep,
		include_directories : [inc],
		link_with : [libhuffman])

test('encoder', e)