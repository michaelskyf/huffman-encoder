#include <iostream>
#include <fstream>
#include <spdlog/spdlog.h>
#include <argparse/argparse.hpp>

#include "HuffmanCompressor.hpp"
#include "JsonHuffmanParser.hpp"

#ifndef VERSION
#define VERSION "unknown"
#endif

struct ParsedArguments
{
	enum {Compression, Decompression} mode;
	std::string input_path;
	std::string output_path;
	std::string dictionary_path;
};

namespace
{

std::pair<bool, ParsedArguments> parse_arguments(int argc, char* argv[])
{
	argparse::ArgumentParser parser(*argv, VERSION);
	ParsedArguments result;

	parser.add_argument("--input", "-i")
		.required()
		.help("specify the input file");

	parser.add_argument("--output", "-o")
		.default_value("-")
		.help("specify the output file");

	parser.add_argument("--mode", "-m", "-t")
		.required()
		.help("specify the mode ('c' or 'd')");

	parser.add_argument("--dictionary", "-d", "-s")
		.required()
		.help("specify the dictionary file");

	try
	{
    	parser.parse_args(argc, argv);
  	}
  	catch(const std::runtime_error& err)
	{
    	spdlog::error(err.what());
    	return {false, {}};
 	}

	auto mode = parser.get("mode");
	if(mode == "c")
	{
		result.mode = ParsedArguments::Compression;
	}
	else if(mode == "d")
	{
		result.mode = ParsedArguments::Decompression;
	}
	else
	{
		spdlog::error("Invalid mode '{}'", mode);
		return {false, {}};
	}

	result.input_path = parser.get("input");
	result.output_path = parser.get("output");
	result.dictionary_path = parser.get("dictionary");

	return {true, result};
}

int compress(const ParsedArguments& args)
{
	HuffmanCompressor compressor;
	std::ifstream input(args.input_path);
	std::ofstream output(args.output_path);
	std::ofstream dictionary(args.dictionary_path);

	if(!input || !output || !dictionary)
	{
		spdlog::error("{}: Failed to open input, output or dictionary", __func__);
		return 1;
	}

	if(compressor.compress(input, output) == false)
	{
		spdlog::error("{}: Failed to compress data", __func__);
		return 1;
	}

	if(jhparser::write_dictionary(dictionary, compressor.data()) == false)
	{
		spdlog::error("{}: Failed to write dictionary", __func__);
		return 1;
	}

	input.close();
	output.close();
	dictionary.close();
	return 0;
}

int decompress(const ParsedArguments& args)
{
	std::ifstream input(args.input_path);
	std::ofstream output(args.output_path);
	std::ifstream dictionary(args.dictionary_path);

	if(!input || !output || !dictionary)
	{
		spdlog::error("{}: Failed to open input, output or dictionary", __func__);
		return 1;
	}

	HuffmanCompressor decompressor(jhparser::read_dictionary(dictionary));
	dictionary.close();
	if(decompressor.empty())
	{
		spdlog::error("{}: Failed to read dictionary", __func__);
		return 1;
	}

	if(decompressor.decompress(input, output) == false)
	{
		spdlog::error("{}: Failed to decompress data", __func__);
		return 1;
	}

	input.close();
	output.close();
	return 0;
}

} // namespace

int main(int argc, char* argv[])
{
	auto[was_successful, args] = parse_arguments(argc, argv);
	if(was_successful == false)
	{
		return 1;
	}

	if(args.mode == ParsedArguments::Compression)
	{
		return compress(args);
	}
	else
	{
		return decompress(args);
	}
}
