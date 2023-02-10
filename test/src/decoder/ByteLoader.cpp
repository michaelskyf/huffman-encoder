#include <decoder/ByteLoader.hpp>
#include <gtest/gtest.h>

using namespace huffman::decoder;

TEST(decoder_ByteLoader, empty)
{
	auto object = ByteLoader(nullptr, 0, 0);

	EXPECT_TRUE(object.empty());
	EXPECT_EQ(object.bitsProcessed(), 0);
	EXPECT_EQ(object.totalBits(), 0);
}

TEST(decoder_ByteLoader, empty_offset)
{
	auto object = ByteLoader(nullptr, 0, 12);

	EXPECT_TRUE(object.empty());
	EXPECT_EQ(object.bitsProcessed(), 0);
	EXPECT_EQ(object.totalBits(), 0);
}

TEST(decoder_ByteLoader, one_byte)
{
	char byte = '\xB3';
	auto object = ByteLoader(&byte, 1, 3);

	EXPECT_FALSE(object.empty());
	EXPECT_EQ(object.value() & 0b111, (byte >> 3) & 0b111);
	EXPECT_EQ(object.bitsProcessed(), 3);
	EXPECT_EQ(object.totalBits(), 8);
}

TEST(decoder_ByteLoader, one_byte_shift)
{
	char byte = '\xB3';
	auto object = ByteLoader(&byte, 1, 0);

	object >>= 3;

	EXPECT_FALSE(object.empty());
	EXPECT_EQ(object.value() & 0b111, (byte >> 3) & 0b111);
	EXPECT_EQ(object.bitsProcessed(), 3);
	EXPECT_EQ(object.totalBits(), 8);
}

TEST(decoder_ByteLoader, one_byte_big_offset)
{
	char byte = '\xB3';
	auto object = ByteLoader(&byte, 1, 9);

	EXPECT_TRUE(object.empty());
	EXPECT_EQ(object.bitsProcessed(), 8);
	EXPECT_EQ(object.totalBits(), 8);
}

TEST(decoder_ByteLoader, one_byte_big_shift)
{
	char byte = '\xB3';
	auto object = ByteLoader(&byte, 1, 0);

	object >>= 9;

	EXPECT_TRUE(object.empty());
	EXPECT_EQ(object.bitsProcessed(), 8);
	EXPECT_EQ(object.totalBits(), 8);
}

TEST(decoder_ByteLoader, byte_array_shift)
{
	std::string bytes{ '\xAA', '\xBB', '\xAA', '\xBB' };
	auto object = ByteLoader(bytes.data(), bytes.size(), 0);

	object >>= 16;

	EXPECT_FALSE(object.empty());
	EXPECT_EQ(object.bitsProcessed(), 16);
	EXPECT_EQ(object.totalBits(), bytes.size()*8);
	EXPECT_EQ('\xAA', object.value());
}

TEST(decoder_ByteLoader, byte_array_offset)
{
	std::string bytes{ '\xAA', '\xBB', '\xAA', '\xBB' };
	auto object = ByteLoader(bytes.data(), bytes.size(), 16);

	EXPECT_FALSE(object.empty());
	EXPECT_EQ(object.bitsProcessed(), 16);
	EXPECT_EQ(object.totalBits(), bytes.size()*8);
	EXPECT_EQ('\xAA', object.value());
}