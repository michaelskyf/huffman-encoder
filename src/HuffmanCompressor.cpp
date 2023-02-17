#include "HuffmanCompressor.hpp"

#include <istream>
#include <array>
#include <sstream>
#include <cstring>

namespace
{

huffman::HuffmanDictionary init_dictionary(std::istream& src)
{
	std::string rdbuf;
	huffman::HuffmanDictionary result{};

	while(std::getline(src, rdbuf))
	{
		result.create_part(rdbuf.data(), rdbuf.size());
	}

	src.clear();
	src.seekg(src.beg);
	return result;
}

class Reader
{
public:
	Reader(std::istream& src)
		: m_src{src}
	{

	}

	size_t buffered_size() const
	{
		return m_bytes_used;
	}

	char* data()
	{
		return m_buffer.data();
	}

	size_t read()
	{
		m_src.read(data() + buffered_size(), static_cast<std::streamsize>(m_buffer.size() - buffered_size()));
		
		m_bytes_used += static_cast<size_t>(m_src.gcount());

		return m_bytes_used;
	}

	void erase(size_t n)
	{
		if(n > m_bytes_used)
		{
			n = m_bytes_used;
		}

		memmove(m_buffer.data(), m_buffer.data() + n, buffered_size() - n);

		m_bytes_used -= n;
	}

private:
	std::istream& m_src;
	std::array<char, 1024> m_buffer;
	size_t m_bytes_used = 0;
};

} // namespace

HuffmanCompressor::HuffmanCompressor(huffman::HuffmanDictionary&& dictionary)
	: m_dictionary{std::move(dictionary)}
{
	
}

HuffmanCompressor::HuffmanCompressor(const huffman::HuffmanDictionary& dictionary)
	: m_dictionary{dictionary}
{

}

bool HuffmanCompressor::compress(std::istream& src, std::ostream& dst)
{
	m_dictionary = init_dictionary(src);
	std::string wrbuf(3, '\0');
	Reader reader(src);
	Writer writer;
	size_t offset = 0;

	while(true)
	{
		auto[src_read, bits_written] = m_dictionary.encode(reader.data(), reader.read(), wrbuf.data(), wrbuf.size(), offset);
		if(bits_written == offset)
		{
			break;
		}

		reader.erase(src_read);
		offset = bits_written % 8;

		size_t bytes_to_write = bits_written/8;
		if(!dst.write(wrbuf.data(), static_cast<std::streamsize>(bytes_to_write)))
		{
			return false;
		}

		if(offset)
		{
			wrbuf[0] = wrbuf[bytes_to_write];
		}
	}

	if(offset)
	{
		if(!dst.write(wrbuf.data(), 1))
		{
			return false;
		}
	}

	return true;
}

bool HuffmanCompressor::decompress(std::istream& src, std::ostream& dst)
{

	return true;
}

const huffman::HuffmanDictionary& HuffmanCompressor::data() const
{
	return m_dictionary;
}

bool HuffmanCompressor::empty() const
{
	return m_dictionary.empty();
}

size_t HuffmanCompressor::size() const
{
	return m_dictionary.size();
}