#pragma once

#include <cstddef>
#include <cstdint>

namespace huffman::encoder
{

class ByteWriter
{
public:
	ByteWriter(char* dst, size_t dst_size, size_t offset);

	size_t bitsWritten() const;
	size_t maxBits() const;

	bool empty() const;

	bool write(uint64_t code, size_t length);

private:
	char* m_dst;
	size_t m_bits_written;
	size_t m_shift;
	const size_t m_total_bits;
};

} // namespace huffman::encoder