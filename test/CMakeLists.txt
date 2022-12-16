set(TEST_EXECUTABLE "${PROJECT_NAME}-test")

add_executable(
	${TEST_EXECUTABLE}
	main.cpp
	../src/huffman.cpp
	)

target_link_libraries(
	${TEST_EXECUTABLE}
	GTest::GTest
	GTest::gtest_main
	)

gtest_discover_tests(${TEST_EXECUTABLE})

find_program(VALGRIND "valgrind")
if(VALGRIND)

	set(MEMORYCHECK_COMMAND_OPTIONS "--max-threads=1000 --leak-check=full --error-exitcode=1 --track-origins=yes")
	# include Dart to generate the site configuration:
	# https://gitlab.kitware.com/cmake/community/wikis/doc/ctest/Generating-Testing-Files#using-cmake
	include(Dart)

endif()
