#include "HuffmanCompressor.hpp"

#include <istream>
#include <array>
#include <sstream>
#include <cstring>

namespace
{

huffman::HuffmanDictionary init_dictionary(std::istream& src)
{
	std::array<char, 1024> rdbuf;
	huffman::HuffmanDictionary result{};

	while(true)
	{
		src.read(rdbuf.data(), rdbuf.size());
		if(src.gcount() == 0)
		{
			break;
		}

		result.create_part(rdbuf.data(), static_cast<size_t>(src.gcount()));
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
	std::array<char, 4> m_buffer;
	size_t m_bytes_used = 0;
};

class Writer
{
public:

	Writer(std::ostream& dst)
		: m_dst{dst}
	{

	}

	size_t space_used() const
	{
		return m_bytes_used;
	}

	size_t space_left() const
	{
		return m_buffer.size()-space_used();
	}

	char* buffer()
	{
		return m_buffer.data()+space_used();
	}

	bool write()
	{
		if(!m_dst.write(m_buffer.data(), static_cast<std::streamsize>(space_used())))
		{
			return false;
		}

		memmove(m_buffer.data(), buffer(), space_left());

		m_bytes_used = 0;
		return true;
	}

	void set_written(size_t n)
	{
		if(n > space_left())
		{
			n = space_left();
		}

		m_bytes_used += n;
	}

private:
	std::ostream& m_dst;
	std::array<char, 4> m_buffer;
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
	if(m_dictionary.empty())
	{
		m_dictionary = init_dictionary(src);
	}
	
	Reader reader(src);
	Writer writer(dst);
	size_t offset = 0;

	while(true)
	{
		auto[src_read, bits_written] = m_dictionary.encode(reader.data(), reader.read(), writer.buffer(), writer.space_left(), offset);
		if(bits_written == offset)
		{
			break;
		}

		size_t bytes_written = bits_written / 8;
		offset = bits_written % 8;

		reader.erase(src_read);
		writer.set_written(bytes_written);

		if(!writer.write())
		{
			return false;
		}
	}

	if(offset)
	{
		writer.set_written(1);
		if(!writer.write())
		{
			return false;
		}
	}

	return true;
}

bool HuffmanCompressor::decompress(std::istream& src, std::ostream& dst)
{
	if(m_dictionary.empty())
	{
		return false;
	}

	Reader reader(src);
	Writer writer(dst);
	size_t offset = 0;
	size_t bytes_left = m_dictionary.size();

	while(true)
	{
		auto[src_read, dst_written] = m_dictionary.decode(reader.data(), reader.read(), writer.buffer(), std::min(writer.space_left(), bytes_left), offset);
		if(dst_written == 0)
		{
			return true;
		}

		size_t bytes_read = src_read / 8;
		offset = src_read % 8;

		reader.erase(bytes_read);
		writer.set_written(dst_written);
		bytes_left -= dst_written;

		if(!writer.write())
		{
			return false;
		}
	}
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