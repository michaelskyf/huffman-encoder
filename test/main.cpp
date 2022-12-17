#include <cstdio>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <utility>

#include "../src/huffman.hpp"

TEST(huffman, string)
{
	HuffmanCoder coder;
	const std::string test_string = "A" "BB" "CCC" "DDDD" "EEEEE" "FFFFFF" "GGGGGGG";
	const std::vector<unsigned char> correctly_encoded_string = 
	{0xEF, 0x6E, 0x3B, 0x49, 0x00, 0x00, 0xAA, 0x5A, 0x55, 0x01};
	// More common bytes are begin with 0, while less common with 1

	const auto result = coder.encode(test_string);

	if(correctly_encoded_string.size() != result.size())
	{
		FAIL();
	}

	for(size_t i = 0; i < correctly_encoded_string.size(); i++)
	{
		if((unsigned char)result[i] != correctly_encoded_string[i])
		{
			FAIL();
		}
	}
}