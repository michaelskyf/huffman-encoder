#pragma once

#include <huffman/HuffmanDictionary.hpp>

namespace jhparser
{
	huffman::HuffmanDictionary read_dictionary(std::istream& stream);
	bool write_dictionary(std::ostream& stream, const huffman::HuffmanDictionary&);
}