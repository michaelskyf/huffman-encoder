#include <algorithm>
#include <gtest/gtest.h>
#include <string>

#include <huffman.hpp>

/**
 Useful:
#include <vector>
#include <iostream>

std::vector<unsigned char> b = {0x7F, 0xB7, 0x8D, 0x24, 0x01, 0x00, 0x55, 0xA5, 0xAA, 0x02};

int main()
{
	bool carry = 0;

	for(auto it = b.rbegin(); it != b.rend(); it++)
	{
		bool tmp_carry = *it & 1;
		*it = carry << 7 | *it >> 1;

		carry = tmp_carry;
	}

	for(auto c : b)
	{
		printf("0x%.2X, ", c);
	}

	std::cout << std::endl;
}

*/

TEST(huffman, encoding)
{
	HuffmanDictionary coder;
	const std::string test_string = "A" "BB" "CCC" "DDDD" "EEEEE" "FFFFFF" "GGGGGGG";
	const std::vector<unsigned char> correctly_encoded_string =
	{0x7F, 0xB7, 0x8D, 0x24, 0x01, 0x00, 0x55, 0xA5, 0xAA, 0x02};
	std::vector<char> buffer(correctly_encoded_string.size());

	coder.create(test_string.data(), test_string.size());
	size_t offset = coder.encode(test_string.data(), test_string.size(), buffer.data(), buffer.size(), 0).second;

	EXPECT_EQ(offset, (correctly_encoded_string.size()-1)*8+2); // test_string.size()-1 full characters and 2 additional bits
	if(offset != (correctly_encoded_string.size()-1)*8+2)
	{
		FAIL();
	}

	for(size_t i = 0; i < correctly_encoded_string.size(); i++)
	{
		EXPECT_EQ((unsigned char)buffer[i], correctly_encoded_string[i]);
	}
}

TEST(huffman, encoding_offset)
{
	HuffmanDictionary coder;
	const std::string test_string = "A" "BB" "CCC" "DDDD" "EEEEE" "FFFFFF" "GGGGGGG";
	const std::vector<unsigned char> correctly_encoded_string =
	{0xAB, 0xCD, 0xEF, 0xBF, 0xDB, 0x46, 0x92, 0x00, 0x80, 0xAA, 0x52, 0x55, 0x01};
	std::vector<unsigned char> buffer(correctly_encoded_string.size());
	buffer[0] = 0xAB;
	buffer[1] = 0xCD;
	buffer[2] = 0xEF;

	coder.create(test_string.data(), test_string.size());
	size_t offset = coder.encode(test_string.data(), test_string.size(), (char*)buffer.data(), buffer.size(), 23).second;
	EXPECT_EQ(offset, (correctly_encoded_string.size()-1)*8+1); // test_string.size()-1 full characters and 2 additional bits

	if(offset != (correctly_encoded_string.size()-1)*8+1)
	{
		FAIL();
	}

	for(size_t i = 0; i < correctly_encoded_string.size(); i++)
	{
		EXPECT_EQ((unsigned char)buffer[i], correctly_encoded_string[i]);
	}
}

TEST(huffman, encoding_parial_init)
{
	HuffmanDictionary coder;
	size_t total_offset = 0;
	const std::string test_string1 = "EEEEE" "FFFFFF" "GGGGGGG";
	const std::string test_string2 = "A" "BB" "CCC" "DDDD";
	const std::vector<unsigned char> correctly_encoded_string =
	{0xAB, 0xCD, 0xEF, 0xBF, 0xDB, 0x46, 0x92, 0x00, 0x80, 0xAA, 0x52, 0x55, 0x01};
	std::vector<unsigned char> buffer(correctly_encoded_string.size());
	buffer[0] = 0xAB;
	buffer[1] = 0xCD;
	buffer[2] = 0xEF;

	coder.create_part(test_string1.data(), test_string1.size());
	coder.create_part(test_string2.data(), test_string2.size());
	total_offset = coder.encode(test_string2.data(), test_string2.size(), (char*)buffer.data(), buffer.size(), 23).second;
	total_offset = coder.encode(test_string1.data(), test_string1.size(), (char*)buffer.data(), buffer.size(), total_offset).second;
	EXPECT_EQ(total_offset, (correctly_encoded_string.size()-1)*8+1); // test_string.size()-1 full characters and 2 additional bits

	if(total_offset != (correctly_encoded_string.size()-1)*8+1)
	{
		FAIL();
	}

	for(size_t i = 0; i < correctly_encoded_string.size(); i++)
	{
		EXPECT_EQ((unsigned char)buffer[i], correctly_encoded_string[i]);
	}
}

TEST(huffman, encoding_empty)
{
	HuffmanDictionary coder;
	const std::string test_string{};
	char buffer[1024];

	size_t offset = coder.encode(test_string.c_str(), test_string.size(), buffer, sizeof(buffer)).second;

	EXPECT_EQ(coder.is_initialized(), false);
	EXPECT_EQ(offset, 0);
}

TEST(huffman, decoding)
{
	HuffmanDictionary coder;
	const std::string test_string = "A" "BB" "CCC" "DDDD" "EEEEE" "FFFFFF" "GGGGGGG";
	char buffer[1024];
	const std::basic_string<unsigned char> encoded_string =
	{0x7F, 0xB7, 0x8D, 0x24, 0x01, 0x00, 0x55, 0xA5, 0xAA, 0x02};

	coder.create(test_string.c_str(), test_string.size());

	auto decoded_offset = coder.decode((char*)encoded_string.c_str(), encoded_string.size(), buffer, std::min(coder.size(), sizeof(buffer)));

	EXPECT_EQ(test_string.size(), decoded_offset.second);

	if(test_string.size() != decoded_offset.second)
	{
		FAIL();
	}

	for(size_t i = 0; i < test_string.size(); i++)
	{
		EXPECT_EQ(buffer[i], test_string[i]);
	}
}

TEST(huffman, decoding_empty)
{
	HuffmanDictionary coder;
	const std::string test_string{};
	const std::basic_string<unsigned char> encoded_string{};
	char buffer[1024];

	coder.create(test_string.c_str(), test_string.size());

	EXPECT_EQ(coder.is_initialized(), false);

	auto decoded_offset = coder.decode((char*)encoded_string.c_str(), encoded_string.size(), buffer, sizeof(buffer));

	EXPECT_EQ(decoded_offset.second, 0);
}

void test_string(const std::string& original)
{
	HuffmanDictionary dictionary;
	dictionary.create(original.data(), original.size());

	if(!dictionary.is_initialized())
	{
		FAIL();
	}

	std::string encoded;
	encoded.resize(dictionary.size());
	auto encode_result = dictionary.encode(original.data(), original.size(), encoded.data(), encoded.size());
	if(encode_result.first != original.size())
	{
		FAIL();
	}

	std::string decoded;
	decoded.resize(dictionary.size());
	auto decode_result = dictionary.decode(encoded.data(), encoded.size(), decoded.data(), decoded.size());
	if(decode_result.second != dictionary.size())
	{
		FAIL();
	}

	EXPECT_EQ(original, decoded);
}

TEST(huffman, long_text)
{
	const std::string original[] = {
		"Get your motor runnin'\n"
		"Head out on the highway\n"
		"Looking for adventure\n"
		"In whatever comes our way\n"
		"Yeah, darlin' gonna make it happen\n"
		"Take the world in a love embrace\n"
		"Fire all of your guns at once\n"
		"And explode into space\n"
		"I like smoke and lightnin'\n"
		"Heavy metal thunder\n"
		"Racing with the wind\n"
		"And the feeling that I'm under\n"
		"Yeah, darlin' gonna make it happen\n"
		"Take the world in a love embrace\n"
		"Fire all of your guns at once\n"
		"And explode into space\n"
		"Like a true nature's child\n"
		"We were born\n"
		"Born to be wild\n"
		"We can climb so high\n"
		"I never wanna die\n"
		"Born to be wild\n"
		"Born to be wild\n"
		"Get your motor runnin'\n"
		"Head out on the highway\n"
		"Looking for adventure\n"
		"In whatever comes our way\n"
		"Yeah, darlin' gonna make it happen\n"
		"Take the world in a love embrace\n"
		"Fire all of your guns at once\n"
		"And explode into space\n"
		"Like a true nature's child\n"
		"We were born\n"
		"Born to be wild\n"
		"We can climb so high\n"
		"I never wanna die\n"
		"Born to be wild\n"
		"Born to be wild"
		,
		" Morbi tempor tempor semper. Integer ultricies, quam luctus tempor consectetur, quam tortor vehicula enim, vel ullamcorper sapien nulla nec sem. Sed posuere dui quis porttitor vulputate. Ut laoreet sapien libero, eget faucibus enim ultrices et. Nulla facilisi. Pellentesque rutrum sagittis orci at ultricies. Nunc luctus, augue nec lobortis condimentum, dui nunc tincidunt lorem, ut mattis sapien erat eu nibh. Aliquam a mattis eros. Integer ac metus pulvinar, viverra leo non, suscipit ex. Donec in auctor tortor. Vivamus rutrum ut ipsum a venenatis.\n"
"\n"
"Duis libero magna, condimentum quis mauris a, faucibus tincidunt elit. Curabitur sit amet magna ac est venenatis rhoncus at eu augue. Vivamus at lectus condimentum massa commodo consequat non ac ante. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce laoreet a ex consectetur malesuada. Fusce hendrerit enim velit, a varius mi commodo eu. Donec rutrum iaculis arcu at vestibulum. Nunc non malesuada neque. Praesent pulvinar urna quis aliquam dignissim. Donec ac volutpat mauris.\n"
"\n"
"Donec commodo elit ac placerat ullamcorper. Sed pharetra metus sit amet lectus scelerisque maximus. Donec id feugiat orci. Vivamus tortor purus, finibus ac porta ut, consequat sollicitudin nulla. In molestie scelerisque diam fermentum suscipit. Mauris lacinia luctus lorem at sagittis. Mauris maximus diam sed elit egestas, quis vehicula massa condimentum. Vestibulum et placerat dolor. "
		};

	for(const auto& txt : original)
	{
		test_string(txt);
	}
}
