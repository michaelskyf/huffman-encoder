#pragma once

#include <memory>

struct huffman_tree_node
{
	huffman_tree_node() = default;
	huffman_tree_node(char c, size_t freq);
	huffman_tree_node(std::unique_ptr<huffman_tree_node>&& lhs, std::unique_ptr<huffman_tree_node>&& rhs, size_t freq);
	~huffman_tree_node() = default;

	huffman_tree_node(const huffman_tree_node&);
	huffman_tree_node(huffman_tree_node&&);
	huffman_tree_node& operator=(const huffman_tree_node&);
	huffman_tree_node& operator=(huffman_tree_node&&);

	/**
	 * @brief				check if the node i a character node
	 * @returns				true if the node is character node, otherwise false
	 * @throws				nothing
	 */
	bool is_character() const;

	std::unique_ptr<huffman_tree_node> m_left;
	std::unique_ptr<huffman_tree_node> m_right;
	char m_character;
	size_t m_frequency;
};