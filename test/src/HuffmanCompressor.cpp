#include <gtest/gtest.h>
#include <HuffmanCompressor.hpp>

#include <string>

using namespace std::string_literals;

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

TEST(HuffmanCompressor, decompression)
{
	std::string input = "\x7F\xB7\x8D\x24\x01\x00\x55\xA5\xAA\x02";
	std::string output;
	std::stringstream input_stream(input), output_stream(output);
	HuffmanCompressor compressor;

	bool was_successful = compressor.decompress(input_stream, output_stream);

	EXPECT_EQ(was_successful, true);
	EXPECT_EQ(output, "A" "BB" "CCC" "DDDD" "EEEEE" "FFFFFF" "GGGGGGG");
}