#include <JsonHuffmanParser.hpp>
#include <gtest/gtest.h>

using namespace huffman;
using namespace jhparser;

namespace
{

void compare_trees(const HuffmanNode& lhs, const HuffmanNode& rhs)
{
	EXPECT_EQ(lhs.is_byte_node(), rhs.is_byte_node());
	if(lhs.is_byte_node() != rhs.is_byte_node())
	{
		return;
	}

	bool is_byte_node = lhs.is_byte_node();

	if(is_byte_node)
	{
		EXPECT_EQ(lhs.byte(), rhs.byte());
		EXPECT_EQ(lhs.frequency(), rhs.frequency());
	}
	else
	{
		compare_trees(*lhs.left(), *rhs.left());
		compare_trees(*lhs.right(), *rhs.right());
	}
}

} // namespace

TEST(JsonHuffmanParser, empty_tree)
{
	const std::string json =
	"{\n"
		"\"root\": {"
		"}"
	"}";
	std::stringstream json_stream{json};

	auto dictionary = read_dictionary(json_stream);

	EXPECT_TRUE(dictionary.empty());
}

TEST(JsonHuffmanParser, small_tree)
{
	const std::string json =
	"{"
		"\"root\": {"
			"\"left\": {\"character\": \"207\", \"frequency\": \"406\"},"
			"\"right\": {\"character\": \"101\", \"frequency\": \"123\"}"
		"}"
	"}";
	std::cout << json << std::endl;
	std::stringstream json_stream{json};
	HuffmanNode root =
	{
		{'\xCF', 406},
		{'\x65', 123}
	};

	auto dictionary = read_dictionary(json_stream);

	EXPECT_EQ(dictionary.size(), 529);
	compare_trees(dictionary.data(), root);
}