/*#include <HuffmanNode.hpp>

namespace huffman
{

HuffmanNode::HuffmanNode(char c, size_t freq)
	:m_character{c},
	m_frequency{freq}
{

}

HuffmanNode::HuffmanNode(std::unique_ptr<HuffmanNode>&& lhs, std::unique_ptr<HuffmanNode>&& rhs, size_t freq)
	:m_left{std::move(lhs)},
	m_right{std::move(rhs)},
	m_frequency{freq}
{

}

bool HuffmanNode::is_character() const
{
	return !(m_left.get() || m_right.get());
}

HuffmanNode::HuffmanNode(const HuffmanNode& node)
{
	*this = node;
}

HuffmanNode::HuffmanNode(HuffmanNode&& node)
{
	*this = std::move(node);
}

HuffmanNode& HuffmanNode::operator=(const HuffmanNode& node)
{
	m_character = node.m_character;
	m_frequency = node.m_frequency;

	if(node.m_left && node.m_right)
	{
		m_left = std::make_unique<HuffmanNode>(*node.m_left);
		m_right = std::make_unique<HuffmanNode>(*node.m_right);
	}

	return *this;
}

HuffmanNode& HuffmanNode::operator=(HuffmanNode&& node)
{
	m_character = node.m_character;
	m_frequency = node.m_frequency;

	m_left = std::move(node.m_left);
	m_right = std::move(node.m_right);

	return *this;
}

} // namespace huffman*/