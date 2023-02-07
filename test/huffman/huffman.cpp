#include <algorithm>
#include <cstring>
#include <gtest/gtest.h>
#include <string>

#include <HuffmanDictionary.hpp>

using namespace huffman;

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
	std::vector<unsigned char> buffer(correctly_encoded_string.size());

	coder.create(test_string.data(), test_string.size());
	size_t offset = coder.encode(test_string.data(), test_string.size(), (char*)buffer.data(), buffer.size(), 0, 0).second;

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
	{0xEF, 0xBF, 0xDB, 0x46, 0x92, 0x00, 0x80, 0xAA, 0x52, 0x55, 0x01};
	std::vector<unsigned char> buffer(correctly_encoded_string.size());
	buffer[0] = 0xEF;

	coder.create(test_string.data(), test_string.size());
	size_t offset = coder.encode(test_string.data(), test_string.size(), (char*)buffer.data(), buffer.size(), buffer[0], 7).second;
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

TEST(huffman, encoding_empty)
{
	HuffmanDictionary coder;
	const std::string test_string{};
	char buffer[1024];

	size_t offset = coder.encode(test_string.c_str(), test_string.size(), buffer, sizeof(buffer), 0, 0).second;

	EXPECT_EQ(coder.is_initialized(), false);
	EXPECT_EQ(offset, 0);
}

TEST(huffman, decoding)
{
	HuffmanDictionary coder;
	const std::string test_string = "A" "BB" "CCC" "DDDD" "EEEEE" "FFFFFF" "GGGGGGG";
	const std::basic_string<unsigned char> encoded_string =
	{0x7F, 0xB7, 0x8D, 0x24, 0x01, 0x00, 0x55, 0xA5, 0xAA, 0x02};
	std::string decoded_string(test_string.size(), 0);

	coder.create(test_string.c_str(), test_string.size());

	auto[src_read, decoded_size] = coder.decode((char*)encoded_string.c_str(), encoded_string.size(), decoded_string.data(), std::min(coder.size(), decoded_string.size()), 0);

	EXPECT_EQ(test_string.size(), decoded_size);
	EXPECT_EQ(9*8+2, src_read);
	EXPECT_EQ(test_string, decoded_string);
}

TEST(huffman, decoding_offset)
{
	HuffmanDictionary coder;
	std::string test_string = "A" "BB" "CCC" "DDDD" "EEEEE" "FFFFFF" "GGGGGGG";
	const std::basic_string<unsigned char> encoded_string =
	{0x7F, 0xB7, 0x8D, 0x24, 0x01, 0x00, 0x55, 0xA5, 0xAA, 0x02};

	coder.create(test_string.c_str(), test_string.size());
	test_string = "BB" "CCC" "DDDD" "EEEEE" "FFFFFF" "GGGGGGG";
	std::string decoded_string(test_string.size(), 0);

	auto[src_read, decoded_size] = coder.decode((char*)encoded_string.c_str(), encoded_string.size(), decoded_string.data(), std::min(coder.size()-1, decoded_string.size()), 4);

	EXPECT_EQ(test_string.size(), decoded_size);
	EXPECT_EQ(9*8+2, src_read);
	EXPECT_EQ(test_string, decoded_string);
}

TEST(huffman, decoding_empty)
{
	HuffmanDictionary coder;
	const std::string test_string{};
	const std::basic_string<unsigned char> encoded_string{};
	char buffer[1024];

	coder.create(test_string.c_str(), test_string.size());

	EXPECT_EQ(coder.is_initialized(), false);

	auto decoded_offset = coder.decode((char*)encoded_string.c_str(), encoded_string.size(), buffer, sizeof(buffer), 0);

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
	auto encode_result = dictionary.encode(original.data(), original.size(), encoded.data(), encoded.size(), 0, 0);
	if(encode_result.first != original.size())
	{
		FAIL();
	}

	std::string decoded;
	decoded.resize(dictionary.size());
	auto decode_result = dictionary.decode(encoded.data(), encoded.size(), decoded.data(), decoded.size(), 0);
	if(decode_result.second != dictionary.size())
	{
		FAIL();
	}

	EXPECT_EQ(original, decoded);
}

TEST(huffman, long_text)
{
	const std::string original[] = {
		" łążźśćęńóMorbi tempor tempor semper. Integer ultricies, quam luctus tempor consectetur, quam tortor vehicula enim, vel ullamcorper sapien nulla nec sem. Sed posuere dui quis porttitor vulputate. Ut laoreet sapien libero, eget faucibus enim ultrices et. Nulla facilisi. Pellentesque rutrum sagittis orci at ultricies. Nunc luctus, augue nec lobortis condimentum, dui nunc tincidunt lorem, ut mattis sapien erat eu nibh. Aliquam a mattis eros. Integer ac metus pulvinar, viverra leo non, suscipit ex. Donec in auctor tortor. Vivamus rutrum ut ipsum a venenatis.\n"
"\n"
"Duis libero magna, condimentum quis mauris a, faucibus tincidunt elit. Curabitur sit amet magna ac est venenatis rhoncus at eu augue. Vivamus at lectus condimentum massa commodo consequat non ac ante. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce laoreet a ex consectetur malesuada. Fusce hendrerit enim velit, a varius mi commodo eu. Donec rutrum iaculis arcu at vestibulum. Nunc non malesuada neque. Praesent pulvinar urna quis aliquam dignissim. Donec ac volutpat mauris.\n"
"\n"
"Donec commodo elit ac placerat ullamcorper. Sed pharetra metus sit amet lectus scelerisque maximus. Donec id feugiat orci. Vivamus tortor purus, finibus ac porta ut, consequat sollicitudin nulla. In molestie scelerisque diam fermentum suscipit. Mauris lacinia luctus lorem at sagittis. Mauris maximus diam sed elit egestas, quis vehicula massa condimentum. Vestibulum et placerat dolor."
		};

	for(const auto& txt : original)
	{
		test_string(txt);
	}
}

TEST(huffman, split_text_encoding)
{
	std::string original_txt =
		" łążźśćęńóMorbi tempor tempor semper. Integer ultricies, quam luctus tempor consectetur, quam tortor vehicula enim, vel ullamcorper sapien nulla nec sem. Sed posuere dui quis porttitor vulputate. Ut laoreet sapien libero, eget faucibus enim ultrices et. Nulla facilisi. Pellentesque rutrum sagittis orci at ultricies. Nunc luctus, augue nec lobortis condimentum, dui nunc tincidunt lorem, ut mattis sapien erat eu nibh. Aliquam a mattis eros. Integer ac metus pulvinar, viverra leo non, suscipit ex. Donec in auctor tortor. Vivamus rutrum ut ipsum a venenatis.\n"
"\n"
"Duis libero magna, condimentum quis mauris a, faucibus tincidunt elit. Curabitur sit amet magna ac est venenatis rhoncus at eu augue. Vivamus at lectus condimentum massa commodo consequat non ac ante. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce laoreet a ex consectetur malesuada. Fusce hendrerit enim velit, a varius mi commodo eu. Donec rutrum iaculis arcu at vestibulum. Nunc non malesuada neque. Praesent pulvinar urna quis aliquam dignissim. Donec ac volutpat mauris.\n"
"\n"
"Donec commodo elit ac placerat ullamcorper. Sed pharetra metus sit amet lectus scelerisque maximus. Donec id feugiat orci. Vivamus tortor purus, finibus ac porta ut, consequat sollicitudin nulla. In molestie scelerisque diam fermentum suscipit. Mauris lacinia luctus lorem at sagittis. Mauris maximus diam sed elit egestas, quis vehicula massa condimentum. Vestibulum et placerat dolor.";
	
	std::string encoded_txt;
	std::string partially_encoded_txt;
	char read_buffer[3]; // Buffer for reading
	char write_buffer[3]; // Buffer for writing
	
	HuffmanDictionary dictionary(original_txt.data(), original_txt.size());
	encoded_txt.resize(dictionary.size());

	EXPECT_TRUE(dictionary.is_initialized());
	if(!dictionary.is_initialized())
	{
		FAIL();
	}

	// "read" from the original_text
	size_t original_pos = 0;
	size_t original_size_left = original_txt.size();
	size_t offset = 0;
	size_t read_cnt = 0;
	while(true)
	{
		size_t to_read = std::min(original_size_left, sizeof(read_buffer));
		read_cnt = original_txt.copy(read_buffer, to_read, original_pos);
		if(read_cnt == 0)
		{
			break;
		}

		original_pos += read_cnt;
		original_size_left -= read_cnt;

		char* read_buffer_ptr = read_buffer;
		while(read_cnt)
		{

			char byte_save = *write_buffer;
			auto result = dictionary.encode(read_buffer_ptr, read_cnt, write_buffer, sizeof(write_buffer), *write_buffer, offset);
			
			EXPECT_EQ((char)(byte_save << (8-offset)), (char)((*write_buffer) << (8-offset)));

			offset = result.second % 8;

			if(result.first == 0)
			{
				break;
			}
			
			size_t to_write = result.second / 8;

			size_t old_size = partially_encoded_txt.size();
			EXPECT_EQ(partially_encoded_txt.append(write_buffer, to_write).size() - old_size, to_write);

			if(offset)
			{
				memmove(write_buffer, write_buffer+to_write, 1);
			}

			read_cnt -= result.first;
			read_buffer_ptr += result.first;
		}
	}

	if(offset)
	{
		size_t old_size = partially_encoded_txt.size();
		EXPECT_EQ(partially_encoded_txt.append(write_buffer, 1).size() - old_size, 1);
	}

	auto result = dictionary.encode(original_txt.data(), original_txt.size(), encoded_txt.data(), encoded_txt.size(), 0, 0);
	size_t written_bytes = result.second/8 + (result.second % 8 ? 1 : 0);
	encoded_txt.resize(written_bytes);

	EXPECT_EQ(encoded_txt, partially_encoded_txt);
}

TEST(huffman, split_text_decoding)
{
	
	std::string original_txt =
		" łążźśćęńóMorbi tempor tempor semper. Integer ultricies, quam luctus tempor consectetur, quam tortor vehicula enim, vel ullamcorper sapien nulla nec sem. Sed posuere dui quis porttitor vulputate. Ut laoreet sapien libero, eget faucibus enim ultrices et. Nulla facilisi. Pellentesque rutrum sagittis orci at ultricies. Nunc luctus, augue nec lobortis condimentum, dui nunc tincidunt lorem, ut mattis sapien erat eu nibh. Aliquam a mattis eros. Integer ac metus pulvinar, viverra leo non, suscipit ex. Donec in auctor tortor. Vivamus rutrum ut ipsum a venenatis.\n"
"\n"
"Duis libero magna, condimentum quis mauris a, faucibus tincidunt elit. Curabitur sit amet magna ac est venenatis rhoncus at eu augue. Vivamus at lectus condimentum massa commodo consequat non ac ante. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce laoreet a ex consectetur malesuada. Fusce hendrerit enim velit, a varius mi commodo eu. Donec rutrum iaculis arcu at vestibulum. Nunc non malesuada neque. Praesent pulvinar urna quis aliquam dignissim. Donec ac volutpat mauris.\n"
"\n"
"Donec commodo elit ac placerat ullamcorper. Sed pharetra metus sit amet lectus scelerisque maximus. Donec id feugiat orci. Vivamus tortor purus, finibus ac porta ut, consequat sollicitudin nulla. In molestie scelerisque diam fermentum suscipit. Mauris lacinia luctus lorem at sagittis. Mauris maximus diam sed elit egestas, quis vehicula massa condimentum. Vestibulum et placerat dolor. ";
	std::string encoded_txt;
	std::string decoded_txt; // Final result

	HuffmanDictionary dictionary(original_txt.data(), original_txt.size());

	EXPECT_TRUE(dictionary.is_initialized());
	if(!dictionary.is_initialized())
	{
		FAIL();
	}

	// Encode the string
	encoded_txt.resize(dictionary.size());
	auto result = dictionary.encode(original_txt.data(), original_txt.size(), encoded_txt.data(), encoded_txt.size(), 0, 0);
	size_t written_bytes = result.second/8 + (result.second % 8 ? 1 : 0);
	encoded_txt.resize(written_bytes);

	
	std::stringstream stream(encoded_txt);
	std::string read_buf;
	std::string write_buf(3, 0);
	size_t offset = 0;
	size_t chars_left = dictionary.size();

	while(chars_left)
	{
		std::string tmp(3, 0);
		stream.read(tmp.data(), tmp.size());
		if(stream.gcount() == 0)
		{
			break;
		}

		read_buf.append(tmp);

		while(true)
		{
			auto[src_read, dst_written] = dictionary.decode(read_buf.data(), read_buf.size(), write_buf.data(), std::min(write_buf.size(), chars_left), offset);
			if(dst_written == 0)
			{
				break;
			}

			offset = src_read % 8;

			read_buf.erase(0, src_read/8);

			decoded_txt.append(write_buf.data(), dst_written);
			chars_left -= dst_written;
		}
	}

	EXPECT_EQ(original_txt, decoded_txt);
}