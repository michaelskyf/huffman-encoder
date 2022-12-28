#pragma once

#include <memory>
#include <string>

class HuffmanDictionary;

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

class HuffmanDictionary
{
public:
	HuffmanDictionary() = default;
	HuffmanDictionary(const huffman_tree_node& root);
	HuffmanDictionary(const char* data, size_t size);
	~HuffmanDictionary() = default;

	HuffmanDictionary(const HuffmanDictionary&);
	HuffmanDictionary(HuffmanDictionary&&);
	HuffmanDictionary& operator=(const HuffmanDictionary&);
	HuffmanDictionary& operator=(HuffmanDictionary&&);

	/**
	 * @brief				create a new dictionary from the given data
	 * @param[in]	data	pointer to data
	 * @param[in]	size	size of data
	 * @throws				std::bad_alloc
	 */
	void create(const char* data, size_t size);
	
	/**
	 * @brief				get number of characters (sum of all frequencies) in the dictionary
	 * @returns 			0 if the tree is not initialized, otherwise number of characters in the tree
	 * @throws				nothing
	 */
	size_t size() const;

	/**
	 * @brief				check if the dictionary is initialized ( same as size() )
	 * @returns				true if initialized, otherwise false
	 * @throws				nothing
	 */
	bool is_initialized() const;

	/**
	 * @brief				TODO
	 * @returns				nullptr if the tree is not initialized, otherwise a pointer to the root node
	 * @throws				nothing
	 */
	const huffman_tree_node* data() const;

	/**
	 * @brief				create a new dictionary and encode the data according to it
	 * @param[in]	src		data
	 * @returns				encoded data
	 * @throws				std::bad_alloc
	 */
	size_t encode(const char* src, size_t src_size, char* dst, size_t dst_size, size_t offset);

	/**
	 * @brief				decode given data using the dictionary
	 * @param[in]	src		encoded data
	 * @returns				decoded data
	 * @throws				std::bad_alloc
	 */
	std::string decode(const std::string& src) const;

private:
	std::unique_ptr<huffman_tree_node> m_root;
};