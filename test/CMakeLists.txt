set(TEST_EXECUTABLE "${PROJECT_NAME}-test")

add_executable(
	${TEST_EXECUTABLE}
	main.cpp
	)

target_link_libraries(
	${TEST_EXECUTABLE}
	PRIVATE
	${PROJECT_NAME}-lib
	GTest::GTest
	GTest::gtest_main
	)

find_program(VALGRIND "valgrind")
if(VALGRIND)

	set(MEMORYCHECK_COMMAND_OPTIONS "--max-threads=1000 --leak-check=full --error-exitcode=1 --track-origins=yes")
	# include Dart to generate the site configuration:
	# https://gitlab.kitware.com/cmake/community/wikis/doc/ctest/Generating-Testing-Files#using-cmake
	include(Dart)

endif()

include(GoogleTest)
find_package(GTest REQUIRED)

gtest_discover_tests(${TEST_EXECUTABLE})