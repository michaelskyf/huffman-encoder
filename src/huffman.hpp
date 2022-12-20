#pragma once

#include <cstddef>
#include <memory>
#include <vector>
#include <string>

class HuffmanDictionary;

struct huffman_tree_node
{
	huffman_tree_node() = default;
	huffman_tree_node(char c, size_t freq);
	huffman_tree_node(std::unique_ptr<huffman_tree_node>&& lhs, std::unique_ptr<huffman_tree_node>&& rhs, size_t freq);
	~huffman_tree_node() = default;

	bool is_character() const;

	std::unique_ptr<huffman_tree_node> m_left;
	std::unique_ptr<huffman_tree_node> m_right;
	char m_character;
	size_t m_frequency;
};

class HuffmanDictionary
{
public:
	HuffmanDictionary() = default;
	HuffmanDictionary(const char* data, size_t size);
	~HuffmanDictionary() = default;

	HuffmanDictionary(const HuffmanDictionary&);
	HuffmanDictionary(HuffmanDictionary&&) = default;

	
	void create(const char* data, size_t size);
	size_t size() const;
	bool is_initialized() const;

	/**
	 * @brief				create a new dictionary and encode bytes according to it
	 * @param[in]	src		data
	 * @returns				encoded data
	 */
	std::string encode(const std::string& src);

	/**
	 * @brief				decode given data using internal dictionary
	 * @param[in]	src		encoded data
	 * @returns				decoded data
	 */
	std::string decode(const std::string& src) const;

private:
	std::unique_ptr<huffman_tree_node> m_root;
};