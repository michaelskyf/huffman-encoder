#include <gtest/gtest.h>
#include <HuffmanCompressor.hpp>

#include <string>

using namespace std::string_literals;

TEST(HuffmanCompressor, empty)
{
	HuffmanCompressor compressor{};

	EXPECT_TRUE(compressor.empty());
}

TEST(HuffmanCompressor, not_empty)
{
	HuffmanCompressor compressor{
		{
			{
				{'A', 7},
				{'B', 9}
			}
		}
	};

	EXPECT_FALSE(compressor.empty());
}

TEST(HuffmanCompressor, size)
{
	HuffmanCompressor compressor{
		{
			{
				{'A', 7},
				{'B', 9}
			}
		}
	};

	EXPECT_EQ(compressor.size(), 16);
}

TEST(HuffmanCompressor, data)
{
	HuffmanCompressor compressor{
		{
			{
				{'A', 7},
				{'B', 9}
			}
		}
	};

	EXPECT_TRUE(compressor.data().data().left());
	EXPECT_TRUE(compressor.data().data().right());
}

TEST(HuffmanCompressor, compression)
{
	std::string input = "A" "BB" "CCC" "DDDD" "EEEEE" "FFFFFF" "GGGGGGG";
	std::string expected_output = "\x7F\xB7\x8D\x24\x01\x00\x55\xA5\xAA\x02"s;
	std::stringstream input_stream(input), output_stream;
	HuffmanCompressor compressor;

	bool was_successful = compressor.compress(input_stream, output_stream);

	EXPECT_EQ(was_successful, true);
	EXPECT_EQ(output_stream.str(), expected_output);
}

TEST(HuffmanCompressor, decompression_dictionary_not_initialized)
{
	std::string input = "\x7F\xB7\x8D\x24\x01\x00\x55\xA5\xAA\x02"s;
	std::string expected_output = "A" "BB" "CCC" "DDDD" "EEEEE" "FFFFFF" "GGGGGGG";
	std::stringstream input_stream(input), output_stream;
	HuffmanCompressor decompressor;

	bool was_successful = decompressor.decompress(input_stream, output_stream);

	EXPECT_EQ(was_successful, false);
}

TEST(HuffmanCompressor, decompression)
{
	std::string input = "\x7F\xB7\x8D\x24\x01\x00\x55\xA5\xAA\x02"s;
	std::string expected_output = "A" "BB" "CCC" "DDDD" "EEEEE" "FFFFFF" "GGGGGGG";
	std::stringstream input_stream(input), output_stream;
	HuffmanCompressor decompressor({expected_output.data(), expected_output.size()});

	bool was_successful = decompressor.decompress(input_stream, output_stream);

	EXPECT_EQ(was_successful, true);
	EXPECT_EQ(output_stream.str(), expected_output);
}