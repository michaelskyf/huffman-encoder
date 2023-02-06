#pragma once

#include <algorithm>

class DecodingByteLoader 
{
public:

	DecodingByteLoader(const char* src, size_t src_size, unsigned short offset);

	bool empty() const;

	char value() const;

	size_t bitsProcessed() const;

	size_t totalBits() const;

	char operator>>=(size_t right_shift);

private:
	const char* m_src;
	unsigned short m_shift;
	size_t m_bits_processed;
	const size_t m_total_bits;
};