#include "HuffmanNode.hpp"
#include <decoder/ByteDecoder.hpp>
#include <gtest/gtest.h>
#include <type_traits>

using namespace huffman::decoder;

TEST(decoder_ByteDecoder, decode_null)
{
	ByteLoader loader(nullptr, 0, 0);
	huffman::HuffmanTreeNode root
	{
		std::make_unique<huffman::HuffmanCharacterNode>('a', 1),
		std::make_unique<huffman::HuffmanCharacterNode>('b', 2)
	};

	ByteDecoder decoder(loader, root);

	auto[byte, is_set] = decoder.decode();

	EXPECT_FALSE(is_set);
	EXPECT_EQ(decoder.bitsProcessed(), 0);
}