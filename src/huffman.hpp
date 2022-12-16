#pragma once

#include <cstddef>
#include <memory>
#include <vector>
#include <string>

class HuffmanDictionary;
class HuffmanCoder;

class HuffmanDictionary
{
	friend HuffmanCoder;

public:
	HuffmanDictionary() = default;
	HuffmanDictionary(const char* data, size_t size);
	~HuffmanDictionary() = default;

	HuffmanDictionary(const HuffmanDictionary&);
	HuffmanDictionary(HuffmanDictionary&&) = default;

	
	void create(const char* data, size_t size);
	size_t size() const;
	bool is_initialized() const;

private:
	struct huffman_tree_node
	{
		char character;
		size_t frequency;

		std::unique_ptr<huffman_tree_node> left;
		std::unique_ptr<huffman_tree_node> right;

		bool is_character() const
		{
			return !(left.get() || right.get());
		}
	};

	using sorted_frequencies = std::vector<std::unique_ptr<huffman_tree_node>>;

	sorted_frequencies get_frequencies(const char* data, size_t size) const;

	std::unique_ptr<huffman_tree_node> m_root;
};

class HuffmanCoder
{
public:
	HuffmanCoder() = default;
	~HuffmanCoder() = default;

	/**
	 * @brief				create a new dictionary and encode bytes according to it
	 * @returns				encoded data
	 */
	std::string encode(const std::string& src);

	/**
	 * @brief				decode given data using internal dictionary
	 * @returns				decoded data
	 */
	std::string decode(const std::string& src) const;

private:
	HuffmanDictionary m_dictionary;
};
