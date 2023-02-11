#pragma once

#include <memory>

namespace huffman
{

class HuffmanNode
{
public:
	HuffmanNode(size_t frequency = 0, char byte = 0) noexcept
		: m_frequency{frequency}, m_is_byte_node{true}, m_byte{byte}
	{

	}

	HuffmanNode(HuffmanNode&& left, HuffmanNode&& right)
		: m_frequency{left.m_frequency + right.m_frequency},
		  m_is_byte_node{false},
		  m_left{std::make_unique<HuffmanNode>(std::move(left))},
		  m_right{std::make_unique<HuffmanNode>(std::move(right))}
	{

	}

	HuffmanNode(HuffmanNode&& other) noexcept
		: m_frequency{other.m_frequency},
		  m_is_byte_node{other.m_is_byte_node},
		  m_byte{other.m_byte},
		  m_left{std::move(other.m_left)},
		  m_right{std::move(other.m_right)}
	{

	}

	HuffmanNode(const HuffmanNode& other)
		: m_frequency{other.m_frequency},
		  m_is_byte_node{other.m_is_byte_node},
		  m_byte{other.m_byte},
		  m_left{std::make_unique<HuffmanNode>(std::move(*other.m_left))},
		  m_right{std::make_unique<HuffmanNode>(std::move(*other.m_right))}
	{

	}

	HuffmanNode& operator=(HuffmanNode&& other) noexcept
	{
		m_frequency = other.m_frequency;
		m_is_byte_node = other.m_is_byte_node;
		m_byte = other.m_byte;
		m_left = std::move(other.m_left);
		m_right = std::move(other.m_right);
		
		return *this;
	}

	bool operator>=(const HuffmanNode& other) const
	{
		return m_frequency >= other.m_frequency;
	}

	constexpr bool is_byte_node() const
	{
		return m_is_byte_node;
	}

	constexpr size_t frequency() const
	{
		return m_frequency;
	}

	constexpr char byte() const
	{
		return m_byte;
	}

	const HuffmanNode* left() const
	{
		return m_left.get();
	}

	const HuffmanNode* right() const
	{
		return m_right.get();
	}

private:
	size_t m_frequency{};
	bool m_is_byte_node{};

	char m_byte{};
	std::unique_ptr<HuffmanNode> m_left{}, m_right{};
};

} // namespace huffman