#include <map>
#include <functional>
#include <vector>

#include <huffman.hpp>

using sorted_frequencies = std::vector<std::unique_ptr<huffman_tree_node>>;

huffman_tree_node::huffman_tree_node(char c, size_t freq)
	:m_character{c}, m_frequency{freq}
{

}

huffman_tree_node::huffman_tree_node(std::unique_ptr<huffman_tree_node>&& lhs, std::unique_ptr<huffman_tree_node>&& rhs, size_t freq)
	:m_left{std::move(lhs)}, m_right{std::move(rhs)}, m_frequency{freq}
{

}

bool huffman_tree_node::is_character() const
{
	return !(m_left.get() || m_right.get());
}

sorted_frequencies get_frequencies(const char* data, size_t size)
{
	const void* end = data + size;
	size_t freqs[256]{};
	sorted_frequencies result;

	// Get frequencies of each character
	while(data < end)
	{
		freqs[(unsigned char)*data]++;
		data++;
	}

	// Create nodes for characters with frequency > 0
	for(size_t i = 0; i < 256; i++)
	{
		if(freqs[i] == 0) continue;

		// Insert the node into result
		result.emplace(std::find_if(result.begin(),
								result.end(),
								[&](const std::unique_ptr<huffman_tree_node>& n)
								{ return n->m_frequency >= freqs[i]; }),
								std::make_unique<huffman_tree_node>(i, freqs[i]));
	}

	return result;
}

void HuffmanDictionary::create(const char* data, size_t size)
{
	auto frequencies = get_frequencies(data, size);

	while(frequencies.size() > 1)
	{
		size_t new_frequency = frequencies[0]->m_frequency + frequencies[1]->m_frequency;
		auto new_node = std::make_unique<huffman_tree_node>(std::move(frequencies[0]), std::move(frequencies[1]), new_frequency);

		// Remove the first two nodes
		frequencies.erase(frequencies.begin(), frequencies.begin()+2);

		// Insert the new node
		frequencies.insert(std::find_if(frequencies.begin(),
									frequencies.end(),
									[&](const std::unique_ptr<huffman_tree_node>& n)
									{ return n->m_frequency >= new_frequency; }),
									std::move(new_node));
	}

	if(frequencies.size() != 0)
		m_root = std::move(frequencies.front());
	else
		m_root = {};
}

size_t HuffmanDictionary::size() const
{
	if(!is_initialized()) return 0;

	return m_root->m_frequency;
}

bool HuffmanDictionary::is_initialized() const
{
	return m_root.get();
}

std::string HuffmanDictionary::encode(const std::string& src)
{
	std::string result{};
	std::map<char, std::pair<char, uint8_t>> lookup_table{};

	// When travelling down the tree the code reversed, so we need to reverse it once again
	auto reverse_code = [](char code, uint8_t depth)
	{
		char new_code = 0;
		while(depth--)
		{
			new_code |= (code & 1) << depth;
			code >>= 1;
		}

		return new_code;
	};
	
	// Fills up an std::map that contains information to translate characters into their corresponding code
	std::function<void(const huffman_tree_node& node, char code, uint8_t depth)> make_lookup_table =
	[&](const huffman_tree_node& node, char code, uint8_t depth)
	{
		if(node.is_character())
		{
			lookup_table.emplace(node.m_character, std::make_pair(reverse_code(code, depth), depth));
		}
		else
		{
			make_lookup_table(*node.m_right, code << 1, depth+1);
			make_lookup_table(*node.m_left, (code << 1) | 1, depth+1);
		}
	};

	this->create(src.data(), src.size());

	if(!this->is_initialized()) return {};

	make_lookup_table(*this->m_root, 0, 0);

	uint8_t buf_cnt = 0;
	unsigned char c_buf = 0;
	for(char c : src)
	{
		const auto& code = lookup_table.at(c);

		int space_left = 8 - buf_cnt - code.second;

		c_buf |= code.first << buf_cnt;

		if(space_left <= 0)
		{
			result.push_back(c_buf);
			c_buf = 0 | (code.first >> (8-buf_cnt));
			buf_cnt = -space_left;
		}
		else
		{
			buf_cnt = 8 - space_left;
		}
	}

	if(buf_cnt)
		result.push_back(c_buf);

	return result;
}

std::string HuffmanDictionary::decode(const std::string& src) const
{
	if(!this->is_initialized()) return {};
	
	std::string result{};

	size_t characters_left = this->size();
	const huffman_tree_node* current_node = this->m_root.get();
	for(char c : src)
	{
		uint8_t bits_left = 8;

		while(bits_left != 0)
		{
			if(current_node->is_character())
			{
				result.push_back(current_node->m_character);
				current_node = this->m_root.get();
				if(--characters_left == 0) // When a dictionary is initialized, it has at least one character
				{
					return result;
				}
			}
			else
			{
				if(c & 1)
					current_node = current_node->m_left.get();
				else
					current_node = current_node->m_right.get();

				c >>= 1;
				bits_left--;
			}
		}
	}

	return result;
}