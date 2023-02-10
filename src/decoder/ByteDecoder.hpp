#pragma once

#include <HuffmanNode.hpp>
#include "ByteLoader.hpp"

namespace huffman::decoder
{

class ByteDecoder
{
public:

	ByteDecoder(const ByteLoader& loader, const HuffmanNode& root_node)
		: m_loader{loader}, m_root_node{root_node}
	{

	}

	std::pair<char, bool> decode()
	{
		const HuffmanNode* current_node = &m_root_node;

		while(!m_loader.empty() && !current_node->is_character_node())
		{
			const auto& tree_node = reinterpret_cast<const HuffmanTreeNode*>(current_node);

			if(m_loader.value() & 1)
			{
				current_node = tree_node->left();
			}
			else
			{
				current_node = tree_node->right();
			}

			m_loader >>= 1;
		}

		if(current_node->is_character_node())
		{
			return {static_cast<const HuffmanCharacterNode*>(current_node)->character(), true};
		}
		else
		{
			return {0, false};
		}
	}

	size_t bitsProcessed() const
	{
		return m_loader.bitsProcessed();
	}

private:
	ByteLoader m_loader;
	const HuffmanNode& m_root_node;
};

} // namespace huffman::decoder