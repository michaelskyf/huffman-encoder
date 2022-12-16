#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <utility>

#include "../src/huffman.hpp"
/*
class TestDictionary : public HuffmanDictionary
{
public:
	sorted_frequencies get_frequencies(const char* data, size_t size)
	{
		return HuffmanDictionary::get_frequencies(data, size);
	}
};

TEST(huffman, frequencies)
{
	TestDictionary dic;
	const std::string test_string = "A" "BB" "CCC" "DDDD" "EEEEE" "FFFFFF" "GGGGGGG";
	auto result = dic.get_frequencies(test_string.data(), test_string.size());
	const std::pair<char, size_t> frequencies[]
	{
		{'G', 7},
		{'F', 6},
		{'E', 5},
		{'D', 4},
		{'C', 3},
		{'B', 2},
		{'A', 1},
	};

	if(result.size() != sizeof(frequencies)/sizeof(*frequencies))
		FAIL();

	for(size_t i = 0; i < result.size(); i++)
	{
		if(result[i] != frequencies[i])
			FAIL();
	}
}
*/