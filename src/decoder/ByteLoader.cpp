#include "ByteLoader.hpp"

namespace huffman::decoder
{

ByteLoader::ByteLoader(const char* src, size_t src_size, size_t offset)
	: m_src{src},
	  m_shift{0},
	  m_bits_processed{0},
	  m_total_bits{src_size*8}
{
	*this >>= offset;
}

bool ByteLoader::empty() const
{
	return m_bits_processed >= m_total_bits;
}

char ByteLoader::value() const
{
	return empty() ? '\0' : static_cast<char>(*m_src >> m_shift);
}

size_t ByteLoader::bitsProcessed() const
{
	return m_bits_processed;
}

size_t ByteLoader::totalBits() const
{
	return m_total_bits;
}

char ByteLoader::operator>>=(size_t right_shift)
{
	if(m_bits_processed + right_shift < m_total_bits)
	{
		m_bits_processed += right_shift;
	}
	else
	{
		m_bits_processed = m_total_bits;
	}

	m_src += (m_shift + right_shift) / 8;
	m_shift = (m_shift + right_shift) % 8;

	return value();
}

}
