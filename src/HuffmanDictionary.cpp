#include <array>
#include <cstdint>
#include <cstring>
#include <limits>
#include <map>
#include <functional>
#include <memory>
#include <vector>
#include <iostream>
#include <queue>

#include <HuffmanDictionary.hpp>
#include "HuffmanNode.hpp"
#include "decoder/ByteLoader.hpp"
#include "decoder/ByteDecoder.hpp"

namespace huffman
{

namespace
{

void get_frequencies(std::array<size_t, 256>& array, const char* src, size_t src_size)
{
	for(size_t i = 0; i < src_size; i++)
	{
		unsigned char index = static_cast<unsigned char>(src[i]);

		array[index]++;
	}
}

void get_frequencies(std::array<size_t, 256>& array, const HuffmanNode& node)
{
	if(node.is_character_node())
	{
		const auto& char_node = static_cast<const HuffmanCharacterNode&>(node);
		unsigned char index = static_cast<unsigned char>(char_node.character());
		
		array[index] += char_node.frequency();
	}
	else
	{
		const auto& tree_node = static_cast<const HuffmanTreeNode&>(node);
		get_frequencies(array, *tree_node.left());
		get_frequencies(array, *tree_node.right());
	}
}

std::unique_ptr<HuffmanTreeNode> make_huffman_tree(std::priority_queue<std::pair<size_t, std::unique_ptr<HuffmanNode>>>& frequencies)
{
	while(frequencies.size() > 1)
	{
		std::unique_ptr<HuffmanNode> bottom_nodes[2];

		bottom_nodes[0] = std::move(const_cast<std::unique_ptr<HuffmanNode>&>(frequencies.top().second));
		frequencies.pop();

		bottom_nodes[1] = std::move(const_cast<std::unique_ptr<HuffmanNode>&>(frequencies.top().second));
		frequencies.pop();

		auto new_tree_node = std::make_unique<HuffmanTreeNode>(std::move(bottom_nodes[0]), std::move(bottom_nodes[1]));
		frequencies.emplace(new_tree_node->frequency(), std::move(new_tree_node));
	}

	if(frequencies.size() == 0)
	{
		return {};
	}
	else
	{
		return {};
	}
}

} // unnamed namespace

HuffmanDictionary::HuffmanDictionary(const HuffmanTreeNode& root)
	: m_root{std::make_unique<HuffmanTreeNode>(root)}
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
		m_root = std::make_unique<HuffmanTreeNode>(*node.m_root);
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

HuffmanDictionary::HuffmanDictionary(const char* src, size_t src_size)
{
	create(src, src_size);
}

void HuffmanDictionary::create(const char* src, size_t src_size)
{
	m_root = {};
	create_part(src, src_size);
}

const HuffmanNode* HuffmanDictionary::data() const
{
	return m_root.get();
}

void HuffmanDictionary::create_part(const char* src, size_t src_size)
{
	std::array<size_t, 256> byte_frequencies{};

	// Get frequencies from the data
	get_frequencies(byte_frequencies, src, src_size);

	// Get frequencies from the tree if it exists
	if(!empty())
	{
		get_frequencies(byte_frequencies, *m_root.get());
	}

	// Convert the array to priority_queue
	std::priority_queue<std::pair<size_t, std::unique_ptr<HuffmanNode>>> frequencies;
	for(size_t i = 0; i < byte_frequencies.size(); i++)
	{
		size_t freq = byte_frequencies[i];
		// Trim characters that do not appear
		if(freq > 0)
		{
			frequencies.emplace(freq, std::make_unique<HuffmanCharacterNode>(static_cast<char>(i), freq));
		}
	}

	// Make the new root
	m_root = make_huffman_tree(frequencies);
}

size_t HuffmanDictionary::size() const
{
	if(empty()) return 0;

	return m_root->frequency();
}

bool HuffmanDictionary::empty() const
{
	return !m_root.get();
}

std::pair<size_t, size_t> HuffmanDictionary::encode(const char* src, size_t src_size, char* dst, size_t dst_size, char byte, size_t offset)
{
	if(!this->empty())
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
	if(!this->empty())
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