#pragma once

#include <memory>

namespace huffman
{

/*struct HuffmanNode
{
	HuffmanNode() = default;
	HuffmanNode(char c, size_t freq);
	HuffmanNode(std::unique_ptr<HuffmanNode>&& lhs, std::unique_ptr<HuffmanNode>&& rhs, size_t freq);
	~HuffmanNode() = default;

	HuffmanNode(const HuffmanNode&);
	HuffmanNode(HuffmanNode&&);
	HuffmanNode& operator=(const HuffmanNode&);
	HuffmanNode& operator=(HuffmanNode&&);

	*//**
	 * @brief				check if the node i a character node
	 * @returns				true if the node is character node, otherwise false
	 * @throws				nothing
	 */
	/*bool is_character() const;

	std::unique_ptr<HuffmanNode> m_left{};
	std::unique_ptr<HuffmanNode> m_right{};
	char m_character{};
	size_t m_frequency{};
};*/

class HuffmanNode
{
public:
	HuffmanNode(size_t frequency)
		: m_frequency{frequency}
	{

	}

	virtual ~HuffmanNode() = default;

	virtual constexpr bool is_character_node() const = 0;

	constexpr size_t frequency() const
	{
		return m_frequency;
	}

private:
	size_t m_frequency;
};

class HuffmanCharacterNode : public HuffmanNode
{
public:

	HuffmanCharacterNode(char character, size_t frequency)
		: HuffmanNode{frequency}, m_character{character}
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

class HuffmanTreeNode : public HuffmanNode
{
public:
	HuffmanTreeNode(std::unique_ptr<HuffmanNode> lhs, std::unique_ptr<HuffmanNode> rhs)
		: HuffmanNode{lhs->frequency() + rhs->frequency()}, m_left{std::move(lhs)}, m_right{std::move(rhs)}
	{
		
	}

	constexpr bool is_character_node() const override
	{
		return false;
	}

	constexpr HuffmanNode* left() const
	{
		return m_left.get();
	}

	constexpr HuffmanNode* right() const
	{
		return m_right.get();
	}

private:
	std::unique_ptr<HuffmanNode> m_left, m_right;
};

} // namespace huffman