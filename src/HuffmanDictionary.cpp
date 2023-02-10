#include <cstdint>
#include <cstring>
#include <limits>
#include <map>
#include <functional>
#include <memory>
#include <vector>
#include <iostream>

#include <HuffmanDictionary.hpp>
#include "HuffmanNode.hpp"
#include "decoder/ByteLoader.hpp"
#include "decoder/ByteDecoder.hpp"

namespace huffman
{

using frequencies = std::vector<std::unique_ptr<HuffmanCharacterNode>>;

namespace
{

// When travelling down the tree the code reversed, so we need to reverse it once again
uint64_t reverse_code(uint64_t code, size_t depth)
{
	uint64_t new_code = 0;
	while(depth--)
	{
		new_code |= (code & 1) << depth;
		code >>= 1;
	}

	return new_code;
}

// Returns a byte and bits_set in that byte
std::pair<char, size_t> encode_byte(char byte, size_t bits_set, std::pair<uint64_t , size_t>& code)
{
	char real_byte = static_cast<char>(byte & ~(std::numeric_limits<unsigned char>::max() << bits_set));
	size_t bits_written = std::min(8-bits_set, code.second);

	real_byte |= static_cast<char>(code.first << bits_set);
	code.second -= bits_written;
	code.first >>= bits_written;

	return {real_byte, bits_set + bits_written};
}

std::pair<std::string, size_t> encodeBytes(char byte, size_t bits_set, std::pair<uint64_t , size_t> code)
{
	std::string result;

	while(code.second)
	{
		auto[new_byte, new_bits_set]= encode_byte(byte, bits_set, code);

		byte = new_byte;
		result.push_back(byte);
		bits_set = new_bits_set % 8;
	}

	return {result, bits_set};
}

} // unnamed namespace

HuffmanDictionary::HuffmanDictionary(const HuffmanNode& root)
	: m_root{std::make_unique<HuffmanNode>(root)}
{
	
}

HuffmanDictionary::HuffmanDictionary(const HuffmanDictionary& node)
{
	*this = node;
}

HuffmanDictionary::HuffmanDictionary(HuffmanDictionary&& node)
{
	*this = std::move(node);
}

HuffmanDictionary& HuffmanDictionary::operator=(const HuffmanDictionary& node)
{
	if(node.m_root)
	{
		m_root = std::make_unique<HuffmanNode>(*node.m_root);
	}
	else
	{
		m_root = nullptr;
	}
	
	return *this;
}

HuffmanDictionary& HuffmanDictionary::operator=(HuffmanDictionary&& node)
{
	m_root = std::move(node.m_root);
	
	return *this;
}

HuffmanDictionary::HuffmanDictionary(const char* data, size_t size)
{
	create(data, size);
}

void HuffmanDictionary::create(const char* data, size_t size)
{
	m_root = {};
	create_part(data, size);
}

const HuffmanNode* HuffmanDictionary::data() const
{
	return m_root.get();
}

void HuffmanDictionary::create_part(const char* data, size_t size)
{
	
}

size_t HuffmanDictionary::size() const
{
	if(!is_initialized()) return 0;

	return m_root->frequency();
}

bool HuffmanDictionary::is_initialized() const
{
	return m_root.get();
}

std::pair<size_t, size_t> HuffmanDictionary::encode(const char* src, size_t src_size, char* dst, size_t dst_size, char byte, size_t offset)
{
	if(!this->is_initialized())
	{
		return {0, offset};
	}

	// encoder::ByteEncoder encoder
	for(size_t si = 0; si < src_size; si++)
	{
		/*
		auto[byte, byte_offset] = encoder.encode();
		if(byte_offset == 0)
		{
			return {si, offset}
		}
		*/
	}
}

std::pair<size_t, size_t> HuffmanDictionary::decode(const char* src, size_t src_size, char* dst, size_t dst_size, size_t offset)
{
	if(!this->is_initialized())
	{
		return {offset, 0};
	}

	decoder::ByteLoader loader(src, src_size, offset);
	decoder::ByteDecoder decoder(loader, *m_root);

	for(size_t di = 0; di < dst_size; di++)
	{
		auto[byte, is_set] = decoder.decode();
		if(!is_set)
		{
			return {offset, di};
		}

		dst[di] = byte;
		offset = decoder.bitsProcessed();
	}

	return {offset, dst_size};
}

} // namespace huffman