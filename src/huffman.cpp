#include "huffman.hpp"
#include <algorithm>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>

HuffmanDictionary::sorted_frequencies HuffmanDictionary::get_frequencies(const char* data, size_t size) const
{
	const void* end = data + size;
	size_t freqs[256]{};
	sorted_frequencies result;

	// Get frequencies of each character
	for(; data < end; data++)
	{
		freqs[(unsigned char)*data]++;
	}

	// Create nodes for characters with frequency > 0
	for(size_t i = 0; i < 256; i++)
	{
		if(freqs[i] == 0) continue;

		result.emplace_back(std::make_unique<huffman_tree_node>());
		result.back()->character = i;
		result.back()->frequency = freqs[i];
	}

	// Sort the result ascending
	std::sort(result.begin(),
					result.end(),
					[](const std::unique_ptr<huffman_tree_node>& lhs, const std::unique_ptr<huffman_tree_node>& rhs)
					{ return lhs->frequency < rhs->frequency; });

	

	return result;
}

void HuffmanDictionary::create(const char* data, size_t size)
{
	auto frequencies = get_frequencies(data, size);
	auto get_iter = [](const sorted_frequencies& f, size_t frequency)
	{
		for(auto it = f.begin(); it != f.end(); it++)
		{
			if(it->get()->frequency >= frequency)
			{
				return it;
			}
		}

		return f.end();
	};

	while(frequencies.size() > 1)
	{
		auto new_node = std::make_unique<huffman_tree_node>();
		
		new_node->right = std::move(frequencies[0]);
		new_node->left = std::move(frequencies[1]);
		frequencies.erase(frequencies.begin(), frequencies.begin()+2);

		new_node->frequency = new_node->right->frequency + new_node->left->frequency;

		frequencies.insert(get_iter(frequencies, new_node->frequency), std::move(new_node));
	}

	if(frequencies.size() != 0)
		m_root = std::move(frequencies.front());
	else
		m_root = {};
}

size_t HuffmanDictionary::size() const
{
	if(!is_initialized()) return 0;

	return m_root->frequency;
}

bool HuffmanDictionary::is_initialized() const
{
	return m_root.get();
}


std::string HuffmanCoder::encode(const std::string& src)
{
	m_dictionary.create(src.data(), src.size());
}

std::string HuffmanCoder::decode(const std::string& src) const
{

}