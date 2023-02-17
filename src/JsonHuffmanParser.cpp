#include "JsonHuffmanParser.hpp"
#include <huffman/HuffmanDictionary.hpp>
#include <huffman/HuffmanNode.hpp>
#include <boost/json.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace pt = boost::property_tree;
using namespace huffman;

namespace jhparser
{
	huffman::HuffmanDictionary read_dictionary(std::istream& stream)
	{
		pt::ptree root;
		pt::read_json(stream, root);
		HuffmanNode result{0, 0};

		std::function<HuffmanNode(const pt::ptree&)> recursive_get =
		[&](const pt::ptree& json_node)
		{
			HuffmanNode new_node{0, 0};

			try
			{
				return HuffmanNode{static_cast<char>(json_node.get<unsigned char>("character")), json_node.get<size_t>("frequency")};
			}
			catch(...)
			{
				try
				{
					return HuffmanNode{recursive_get(json_node.get_child("left")), recursive_get(json_node.get_child("right"))};
				}
				catch(...)
				{

					return HuffmanNode{0, 0};
				}
			}

			return new_node;
		};

		try
		{
			auto real_root = root.get_child("root");
			result = recursive_get(real_root);
		}
		catch(...)
		{

		}

		return result;

	}

	bool write_dictionary(std::ostream& stream, const huffman::HuffmanDictionary& dictionary)
	{
		pt::ptree json_root, json_root_node;
		const auto& root_node = dictionary.data();

		std::function<void(pt::ptree&, const HuffmanNode&)> recursive_put =
		[&](pt::ptree& json_node, const HuffmanNode& node)
		{
			if(node.is_byte_node())
			{
				json_node.put("frequency", node.frequency());
				json_node.put("character", static_cast<unsigned char>(node.byte()));
			}
			else
			{
				pt::ptree left, right;

				recursive_put(left, *node.left());
				recursive_put(right, *node.right());

				json_node.add_child("left", left);
				json_node.add_child("right", right);
			}
		};

		recursive_put(json_root_node, root_node);

		json_root.add_child("root", json_root_node);

		pt::write_json(stream, json_root);

		return true;
	}
}