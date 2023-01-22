cmake_minimum_required(VERSION 3.13)

project(huffman-compression LANGUAGES CXX VERSION 0.9.0)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

include(CTest)

# Compiler stuff
set(CMAKE_CXX_STANDARD 20)

# Flags
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wundef -pedantic -Wno-gnu-statement-expression -Wno-pointer-arith")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -gdwarf-4") # Valgrind may have a problem if this is not set
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# Definitions
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVERSION=\\\"${CMAKE_PROJECT_VERSION}\\\"")

# Debug/Release flags
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

set(CMAKE_CXX_FLAGS_RELEASE "")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(docs)