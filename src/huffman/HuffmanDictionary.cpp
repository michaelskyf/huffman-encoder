#include <cstdint>
#include <cstring>
#include <limits>
#include <map>
#include <functional>
#include <memory>
#include <vector>
#include <iostream>

#include <HuffmanDictionary.hpp>
#include "DecodingByteLoader.hpp"

namespace huffman
{

using sorted_frequencies = std::vector<std::unique_ptr<HuffmanNode>>;

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

// Returns a map that contains information unsed in translating characters into their corresponding code
std::map<char, std::pair<uint64_t , size_t>> make_lookup_table(const HuffmanNode& root)
{
	std::map<char, std::pair<uint64_t , size_t>> result;

	std::function<void(const HuffmanNode& node, uint64_t code, size_t depth)> recurse_tree =
	[&](const HuffmanNode& node, uint64_t code, size_t depth)
	{
		if(node.is_character())
		{
			result.emplace(node.m_character, std::make_pair(reverse_code(code, depth), depth));
		}
		else
		{
			recurse_tree(*node.m_right, code << 1, depth+1);
			recurse_tree(*node.m_left, (code << 1) | 1, depth+1);
		}
	};

	recurse_tree(root, 0, 0);

	return result;
}

// Returns a byte and bits_set in that byte
std::pair<unsigned char, size_t> encode_byte(char byte, size_t bits_set, std::pair<uint64_t , size_t>& code)
{
	unsigned char real_byte = byte & ~(std::numeric_limits<unsigned char>::max() << bits_set);
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
		auto new_byte_data = encode_byte(byte, bits_set, code);

		byte = new_byte_data.first;
		result.push_back(byte);
		bits_set = new_byte_data.second % 8;
	}

	return {result, bits_set};
}

std::pair<char, bool> decodeByte(DecodingByteLoader& loader, HuffmanNode* current_node)
{
	while(!loader.empty() && !current_node->is_character())
	{
		if(loader.value() & 1)
		{
			current_node = current_node->m_left.get();
		}
		else
		{
			current_node = current_node->m_right.get();
		}

		loader >>= 1;
	}

	return {current_node->m_character, current_node->is_character()};
}

} // unnamed namespace

sorted_frequencies get_frequencies(const char* data, size_t size)
{
	const void* end = data + size;
	size_t freqs[256]{};
	sorted_frequencies result;

	// Get frequencies of each character
	while(data < end)
	{
		freqs[static_cast<unsigned char>(*data)]++;
		data++;
	}

	// Create nodes for characters with frequency > 0
	for(size_t i = 0; i < 256; i++)
	{
		if(freqs[i] == 0) continue;

		// Insert the node into result
		result.emplace(std::find_if(result.begin(),
								result.end(),
								[&](const std::unique_ptr<HuffmanNode>& n)
								{ return n->m_frequency >= freqs[i]; }),
								std::make_unique<HuffmanNode>(i, freqs[i]));
	}

	return result;
}

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
	sorted_frequencies frequencies = get_frequencies(data, size);
	std::vector<std::pair<char, size_t>> old_frequencies;

	std::function<void(const HuffmanNode& node)> get_old_frequencies =
	[&](const HuffmanNode& node)
	{
		if(node.is_character())
		{
			old_frequencies.emplace_back(node.m_character, node.m_frequency);
		}
		else
		{
			get_old_frequencies(*node.m_left);
			get_old_frequencies(*node.m_right);
		}
	};

	if(is_initialized())
	{
		get_old_frequencies(*m_root);

		for(const auto& old_freq : old_frequencies)
		{
			bool set = false;
			for(auto& new_freq : frequencies)
			{
				if(new_freq->m_character == old_freq.first)
				{
					new_freq->m_frequency += old_freq.second;
					set = true;
					break;
				}
			}

			if(set == false)
			{
				frequencies.emplace(std::find_if(frequencies.begin(),
								frequencies.end(),
								[&](const std::unique_ptr<HuffmanNode>& n)
								{ return n->m_frequency >= old_freq.second; }),
								std::make_unique<HuffmanNode>(old_freq.first, old_freq.second));
			}
		}
	}

	while(frequencies.size() > 1)
	{
		size_t new_frequency = frequencies[0]->m_frequency + frequencies[1]->m_frequency;
		auto new_node = std::make_unique<HuffmanNode>(std::move(frequencies[0]), std::move(frequencies[1]), new_frequency);

		// Remove the first two nodes
		frequencies.erase(frequencies.begin(), frequencies.begin()+2);

		// Insert the new node
		frequencies.insert(std::find_if(frequencies.begin(),
									frequencies.end(),
									[&](const std::unique_ptr<HuffmanNode>& n)
									{ return n->m_frequency >= new_frequency; }),
									std::move(new_node));
	}

	if(frequencies.size() != 0)
		m_root = std::move(frequencies.front());
	else
		m_root = {};
}

size_t HuffmanDictionary::size() const
{
	if(!is_initialized()) return 0;

	return m_root->m_frequency;
}

bool HuffmanDictionary::is_initialized() const
{
	return m_root.get();
}

std::pair<size_t, size_t> HuffmanDictionary::encode(const char* src, size_t src_size, char* dst, size_t dst_size, char byte, size_t old_bits_set)
{
	if(!this->is_initialized())
	{
		return {0, old_bits_set};
	}

	if(dst_size > std::numeric_limits<size_t>::max()/8)
	{
		dst_size = std::numeric_limits<size_t>::max()/8;
	}

	auto lookup_table = make_lookup_table(*this->m_root);

	// si -> source index
	// di -> destination index
	// old_bits_set -> bits set on the last byte during the last write
	size_t di = 0;
	for(size_t si = 0; si < src_size; si++)
	{
		std::pair<uint64_t , size_t> code;
		try
		{
			code = lookup_table.at(src[si]);
		}
		catch(...)
		{
			return {si, di*8+old_bits_set};
		}

		auto[encoded_bytes, bits_set] = encodeBytes(byte, old_bits_set, code);

		// If there's no space to write the data
		if(di+encoded_bytes.size() > dst_size)
		{
			// Don't save the new bytes
			return {si, di*8+old_bits_set};
		}

		// Write the bytes
		encoded_bytes.copy(&dst[di], encoded_bytes.size());
		di += encoded_bytes.size() - (bits_set ? 1 : 0); // If the last byte is not fully written, di should point to it
		byte = encoded_bytes.back();
		old_bits_set = bits_set;
	}

	return {src_size, di*8+old_bits_set};
}

std::pair<size_t, size_t> HuffmanDictionary::decode(const char* src, size_t src_size, char* dst, size_t dst_size, size_t old_bits_set)
{
	if(!this->is_initialized())
	{
		return {old_bits_set, 0};
	}

	DecodingByteLoader loader(src, src_size, old_bits_set);
	for(size_t di = 0; di < dst_size; di++)
	{
		auto[byte, is_set] = decodeByte(loader, m_root.get());
		if(!is_set)
		{
			return {old_bits_set, di};
		}

		dst[di] = byte;
		old_bits_set = loader.bitsProcessed();
	}

	return {old_bits_set, dst_size};
}

} // namespace huffman