#pragma once

#include <huffman/HuffmanDictionary.hpp>

class HuffmanCompressor
{
public:
	HuffmanCompressor() = default;
	HuffmanCompressor(huffman::HuffmanDictionary&&);
	HuffmanCompressor(const huffman::HuffmanDictionary&);

	bool compress(std::istream& src, std::ostream& dst);
	bool decompress(std::istream& src, std::ostream& dst);

	const huffman::HuffmanDictionary& data() const;
	bool empty() const;
	size_t size() const;


private:
	huffman::HuffmanDictionary m_dictionary{};
};