#pragma once

#include <memory>

struct HuffmanNode
{
	HuffmanNode() = default;
	HuffmanNode(char c, size_t freq);
	HuffmanNode(std::unique_ptr<HuffmanNode>&& lhs, std::unique_ptr<HuffmanNode>&& rhs, size_t freq);
	~HuffmanNode() = default;

	HuffmanNode(const HuffmanNode&);
	HuffmanNode(HuffmanNode&&);
	HuffmanNode& operator=(const HuffmanNode&);
	HuffmanNode& operator=(HuffmanNode&&);

	/**
	 * @brief				check if the node i a character node
	 * @returns				true if the node is character node, otherwise false
	 * @throws				nothing
	 */
	bool is_character() const;

	std::unique_ptr<HuffmanNode> m_left;
	std::unique_ptr<HuffmanNode> m_right;
	char m_character;
	size_t m_frequency;
};