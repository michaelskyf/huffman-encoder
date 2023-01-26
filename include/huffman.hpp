#pragma once

#include <memory>
#include <string>

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
	 * @brief				create a new dictionary from the given data (partially)
	 * @param[in]	data	pointer to data
	 * @param[in]	size	size of data
	 * @throws				std::bad_alloc
	 */
	void create_part(const char* data, size_t size);
	
	/**
	 * @brief				get sum of all frequencies in the dictionary
	 * @returns 			0 if the tree is not initialized, otherwise sum of all frequencies in the tree
	 * @throws				nothing
	 */
	size_t size() const;

	/**
	 * @brief				check if the dictionary is initialized ( same as size() != 0 )
	 * @returns				true if initialized, otherwise false
	 * @throws				nothing
	 */
	bool is_initialized() const;

	/**
	 * @brief				return a pointer to the root node
	 * @returns				nullptr if the tree is not initialized, otherwise a pointer to the root node
	 * @throws				nothing
	 */
	const huffman_tree_node* data() const;

	/**
	 * @brief						create a new dictionary (if not already initialized) and encode the data according to it
	 * @param[in]		src			source
	 * @param[in]		src_size	sourcec size
	 * @param[in, out]	dst			destination
	 * @param[in]		dst_size	destination size
	 * @param[in]		offset		numer of bits to skip from the first byte of source
	 * @returns						number of bytes read from src (first) and number of bits written to dst (the last byte may be partially written) (second)
	 * @throws						std::bad_alloc
	 */
	std::pair<size_t, size_t> encode(const char* src, size_t src_size, char* dst, size_t dst_size, size_t offset = 0);

	/**
	 * @brief						decode given data using the dictionary
	 * @param[in]		src			source
	 * @param[in]		src_size	sourcec size
	 * @param[in, out]	dst			destination
	 * @param[in]		dst_size	destination size
	 * @param[in]		src_offset	numer of bits to skip from the first byte of source
	 * @returns						number of bits read from src (first) and number of bytes written to dst (second)
	 * @throws						std::bad_alloc
	 */
	std::pair<size_t, size_t> decode(const unsigned char* src, size_t src_size, char* dst, size_t dst_size, size_t src_offset = 0);

private:
	std::unique_ptr<huffman_tree_node> m_root;
};