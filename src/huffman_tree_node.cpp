#include <huffman_tree_node.hpp>

huffman_tree_node::huffman_tree_node(char c, size_t freq)
	:m_character{c},
	m_frequency{freq}
{

}

huffman_tree_node::huffman_tree_node(std::unique_ptr<huffman_tree_node>&& lhs, std::unique_ptr<huffman_tree_node>&& rhs, size_t freq)
	:m_left{std::move(lhs)},
	m_right{std::move(rhs)},
	m_frequency{freq}
{

}

bool huffman_tree_node::is_character() const
{
	return !(m_left.get() || m_right.get());
}

huffman_tree_node::huffman_tree_node(const huffman_tree_node& node)
{
	*this = node;
}

huffman_tree_node::huffman_tree_node(huffman_tree_node&& node)
{
	*this = std::move(node);
}

huffman_tree_node& huffman_tree_node::operator=(const huffman_tree_node& node)
{
	m_character = node.m_character;
	m_frequency = node.m_frequency;

	if(node.m_left && node.m_right)
	{
		m_left = std::make_unique<huffman_tree_node>(*node.m_left);
		m_right = std::make_unique<huffman_tree_node>(*node.m_right);
	}

	return *this;
}

huffman_tree_node& huffman_tree_node::operator=(huffman_tree_node&& node)
{
	m_character = node.m_character;
	m_frequency = node.m_frequency;

	m_left = std::move(node.m_left);
	m_right = std::move(node.m_right);

	return *this;
}