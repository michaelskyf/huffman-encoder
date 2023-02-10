#include "HuffmanNode.hpp"
#include <decoder/ByteDecoder.hpp>
#include <gtest/gtest.h>

using namespace huffman::decoder;

TEST(decoder_ByteDecoder, decode_empty)
{
	ByteLoader loader(nullptr, 0, 0);
	huffman::HuffmanTreeNode empty_node(nullptr, nullptr);

	ByteDecoder decoder(loader, empty_node);

	auto[byte, is_set] = decoder.decode();

	EXPECT_FALSE(is_set);
	EXPECT_EQ(decoder.bitsProcessed(), 0);
}