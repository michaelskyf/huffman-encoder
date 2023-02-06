#pragma once

#include <algorithm>

class DecodingByteLoader 
{
public:

	DecodingByteLoader(const char* src, size_t src_size, unsigned short offset)
		: m_src{src},
		  m_shift{0},
		  m_bits_processed{0},
		  m_total_bits{src_size*8}
	{
		*this >>= offset;
	}

	bool empty() const
	{
		return m_bits_processed >= m_total_bits;
	}

	char value() const
	{
		return empty() ? 0 : *m_src >> m_shift;
	}

	size_t bitsProcessed() const
	{
		return m_bits_processed;
	}

	size_t totalBits() const
	{
		return m_total_bits;
	}

	char operator>>=(size_t right_shift)
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

private:
	const char* m_src;
	unsigned short m_shift;
	size_t m_bits_processed;
	const size_t m_total_bits;
};