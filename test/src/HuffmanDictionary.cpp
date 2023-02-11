#include <HuffmanDictionary.hpp>
#include <gtest/gtest.h>

using namespace huffman;

TEST(HuffmanDictionary, create_empty)
{
	HuffmanDictionary dictionary;
	
	dictionary.create(nullptr, 0);

	EXPECT_TRUE(dictionary.empty());
	EXPECT_EQ(dictionary.size(), 0);
}

TEST(HuffmanDictionary, default_constructor)
{
	HuffmanDictionary dictionary;

	EXPECT_TRUE(dictionary.empty());
	EXPECT_EQ(dictionary.size(), 0);
}

TEST(HuffmanDictionary, constructor)
{
	std::string test_data = "A" "BB" "CCC" "DDDD" "EEEEE" "FFFFFF" "GGGGGGG";
	HuffmanDictionary dictionary(test_data.data(), test_data.size());

	EXPECT_FALSE(dictionary.empty());
	EXPECT_EQ(dictionary.size(), 28);
}

TEST(huffman, decoding)
{
	const std::string test_string = "A" "BB" "CCC" "DDDD" "EEEEE" "FFFFFF" "GGGGGGG";
	const std::basic_string<char> encoded =
	{'\x7F', '\xB7', '\x8D', '\x24', '\x01', '\x00', '\x55', '\xA5', '\xAA', '\x02'};
	std::string decoded(test_string.size(), 0);

	HuffmanDictionary coder(test_string.c_str(), test_string.size());

	auto[src_read, decoded_size] = coder.decode(encoded.c_str(), encoded.size(),
												decoded.data(), std::min(coder.size(), decoded.size()), 0);

	EXPECT_EQ(test_string.size(), decoded_size);
	EXPECT_EQ(9*8+2, src_read);
	EXPECT_EQ(test_string, decoded);
}

TEST(huffman, encoding)
{
	HuffmanDictionary coder;
	const std::string test_string = "A" "BB" "CCC" "DDDD" "EEEEE" "FFFFFF" "GGGGGGG";
	const std::string correctly_encoded_string =
	{'\x7F', '\xB7', '\x8D', '\x24', '\x01', '\x00', '\x55', '\xA5', '\xAA', '\x02'};
	std::string buffer(correctly_encoded_string.size(), 0);

	coder.create(test_string.data(), test_string.size());
	size_t offset = coder.encode(test_string.data(), test_string.size(), buffer.data(), buffer.size(), 0).second;

	EXPECT_EQ(offset, (correctly_encoded_string.size()-1)*8+2); // test_string.size()-1 full characters and 2 additional bits
	if(offset != (correctly_encoded_string.size()-1)*8+2)
	{
		FAIL();
	}

	for(size_t i = 0; i < correctly_encoded_string.size(); i++)
	{
		EXPECT_EQ(buffer[i], correctly_encoded_string[i]);
	}
}