#include <gtest/gtest.h>
#include <string>

#include <huffman.hpp>

TEST(huffman, encoding)
{
	HuffmanDictionary coder;
	const std::string test_string = "A" "BB" "CCC" "DDDD" "EEEEE" "FFFFFF" "GGGGGGG";
	const std::vector<unsigned char> correctly_encoded_string = 
	{0x7F, 0xB7, 0x8D, 0x24, 0x01, 0x00, 0x55, 0xA5, 0xAA, 0x02};
	// More frequent bytes begin with 0, and less common with 1

	const auto encoded_string = coder.encode(test_string);

	EXPECT_EQ(correctly_encoded_string.size(), encoded_string.size());

	if(correctly_encoded_string.size() != encoded_string.size())
	{
		FAIL();
	}

	for(size_t i = 0; i < correctly_encoded_string.size(); i++)
	{
		EXPECT_EQ((unsigned char)encoded_string[i], correctly_encoded_string[i]);
	}
}

TEST(huffman, encoding_empty)
{
	HuffmanDictionary coder;
	const std::string test_string{};

	const auto encoded_string = coder.encode(test_string);

	EXPECT_EQ(coder.is_initialized(), false);
	EXPECT_EQ(encoded_string.size(), 0);
}

TEST(huffman, decoding)
{
	HuffmanDictionary coder;
	const std::string test_string = "A" "BB" "CCC" "DDDD" "EEEEE" "FFFFFF" "GGGGGGG";
	const std::basic_string<unsigned char> encoded_string = 
	{0x7F, 0xB7, 0x8D, 0x24, 0x01, 0x00, 0x55, 0xA5, 0xAA, 0x02};

	coder.encode(test_string); // Creates a huffman tree inside the coder

	const auto decoded_string = coder.decode({encoded_string.begin(), encoded_string.end()});

	EXPECT_EQ(test_string.size(), decoded_string.size());

	if(test_string.size() != decoded_string.size())
	{
		FAIL();
	}

	for(size_t i = 0; i < test_string.size(); i++)
	{
		EXPECT_EQ(decoded_string[i], test_string[i]);
	}
}

TEST(huffman, decoding_empty)
{
	HuffmanDictionary coder;
	const std::string test_string{};
	const std::basic_string<unsigned char> encoded_string{};

	coder.encode(test_string);

	EXPECT_EQ(coder.is_initialized(), false);

	const auto decoded_string = coder.decode({encoded_string.begin(), encoded_string.end()});

	EXPECT_EQ(decoded_string.size(), 0);
}