#pragma once

#include <memory>

namespace huffman
{

class HuffmanNode
{
public:
	HuffmanNode(size_t frequency)
		: m_frequency{frequency}
	{

	}

	HuffmanNode(const HuffmanNode&) = delete;

	virtual ~HuffmanNode() = default;

	virtual constexpr bool is_character_node() const = 0;

	constexpr size_t frequency() const
	{
		return m_frequency;
	}

private:
	size_t m_frequency;
};

class HuffmanCharacterNode final : public HuffmanNode
{
public:

	HuffmanCharacterNode(char character, size_t frequency)
		: HuffmanNode{frequency}, m_character{character}
	{

	}

	HuffmanCharacterNode(const HuffmanCharacterNode& other)
		: HuffmanNode{other.frequency()}, m_character{other.m_character}
	{

	}

	constexpr bool is_character_node() const override
	{
		return true;
	}

	constexpr char character() const
	{
		return m_character;
	}

private:
	char m_character;
};

class HuffmanTreeNode final : public HuffmanNode
{
public:
	HuffmanTreeNode(std::unique_ptr<HuffmanNode>&& lhs, std::unique_ptr<HuffmanNode>&& rhs)
		: HuffmanNode{lhs->frequency() + rhs->frequency()}, m_left{std::move(lhs)}, m_right{std::move(rhs)}
	{
		
	}

	HuffmanTreeNode(const HuffmanTreeNode& other)
		: HuffmanNode{other.frequency()}
	{
		if(other.left()->is_character_node())
		{
			m_left = std::make_unique<HuffmanCharacterNode>(static_cast<const HuffmanCharacterNode&>(*other.left()));
		}
		else
		{
			m_left = std::make_unique<HuffmanTreeNode>(static_cast<const HuffmanTreeNode&>(*other.left()));
		}
	}

	constexpr bool is_character_node() const override
	{
		return false;
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
	std::unique_ptr<HuffmanNode> m_left, m_right;
};

} // namespace huffman