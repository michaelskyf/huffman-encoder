add_library(
	${PROJECT_NAME}-lib
	STATIC
	huffman.cpp
	huffman_tree_node.cpp
	DecodingByteLoader.cpp
)

add_executable(
	${PROJECT_NAME}
	main.cpp
)

target_link_libraries(
	${PROJECT_NAME}
	PRIVATE
	${PROJECT_NAME}-lib
)
