#include <encoder/ByteWriter.hpp>
#include <gtest/gtest.h>

using namespace huffman;
using namespace huffman::encoder;

TEST(ByteWriter, empty)
{
	ByteWriter writer(nullptr, 0, 0);

	EXPECT_FALSE(writer.write(0, 17));

	EXPECT_EQ(writer.bitsWritten(), 0);
	EXPECT_EQ(writer.maxBits(), 0);
	EXPECT_TRUE(writer.empty());
}

TEST(ByteWriter, empty_offset)
{
	ByteWriter writer(nullptr, 0, 9);

	EXPECT_FALSE(writer.write(0, 17));

	EXPECT_EQ(writer.bitsWritten(), 0);
	EXPECT_EQ(writer.maxBits(), 0);
	EXPECT_TRUE(writer.empty());
}