/*
Program 13 - Huffman
Napisać program do kompresji plików metodą Huffmanna.
Program uruchamiany jest z linii poleceń z wykorzystaniem następujących przełączników:
-i plik wejściowy
-o plik wyjściowy
-t tryb: k – kompresja, d – dekompresja
-s plik ze słownikiem (tworzonym w czasie kompresji, używanym w czasie
dekompresji)
*/
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <getopt.h>
#include "huffman.hpp"

#ifndef VERSION
#define VERSION "unknown"
#endif

static struct
{
	const char shortopt;
	const char* longopt;
	int arg_type;
	const char* argname;
	const char* description;
}
options[] =
{
	{'h', "help",			no_argument,		NULL,		"display this message and exit"},
	{'i', "input",			required_argument,	"FILE",		"input file (only one)"},
	{'o', "output",			required_argument,	"FILE",		"output file (only one)"},
	{'m', "mode",			required_argument,	"MODE",		"'k' for compression, 'd' for decompression"},
	{'t', NULL,				required_argument,	"",			"same as --mode (added to meet project's specifications)"},
	{'d', "dictionary",		required_argument,	"FILE",		"file to read/write the dictionary"},
	{'s', NULL,				required_argument,	"FILE",		"same as --dictionary (added to meet project's specifications)"},
	{'v', "version",		no_argument,		NULL,		"show the program's version"},
};

/**
 * @brief
 */
static void print_help(const char* prog_name)
{
	auto print_element = [](const char shortopt, const char* longopt, int arg_type, const char* argname, const char* description)
	{
		std::cout << std::setfill(' ')
			<< std::right << std::setw(4) << (shortopt ? std::string("-") + shortopt : "")
			<< std::left << std::setw(25) << (longopt ? (shortopt ? "," : " ") + std::string(" --") + longopt + (argname ? std::string("=") + argname : "") : "")
			<< description
			<< std::endl;
	};

	std::cout
		<< "Usage: " << prog_name << " options..." << std::endl
		<< "Compress files using Huffman compression." << std::endl
		<< std::endl
		<< "Mandatory arguments to long options are mandatory for short options too." << std::endl;

	for(size_t i = 0; i < sizeof(options)/sizeof(*options); i++)
	{
		auto& opt = options[i];
		print_element(opt.shortopt, opt.longopt, opt.arg_type, opt.argname, opt.description);
	}
}

static int parse_args(int argc, char* argv[])
{
	int option;
	int option_index = 0;
	int should_parse = 1;

	std::vector<struct option> long_options;

	for(size_t i = 0; i < sizeof(options)/sizeof(*options); i++)
	{
		auto& fullopt = options[i];

		long_options.push_back({fullopt.longopt ? fullopt.longopt : "", fullopt.arg_type, 0, fullopt.shortopt});
	}
	long_options.push_back({});

	while(should_parse)
	{
		option = getopt_long(argc, argv, "vhi:o:m:t:d:s:", long_options.data(), &option_index);
		switch(option)
		{
			case 0:
			break;

			case 'h':
				print_help(*argv);
				exit(EXIT_SUCCESS);
			break;

			case 'v':
				std::cout << *argv << ": version " VERSION << std::endl;
				exit(EXIT_SUCCESS);
			break;

			case 'i':
			break;

			case 'o':
			break;

			case 't':
			case 'm':
			break;

			case 's':
			case 'd':
			break;

			case '?':
				std::cerr << "Try '" << *argv << " --help' for more information." << std::endl;
				exit(EXIT_FAILURE);
			break;

			default:
				should_parse = 0;
			break;
		}
	}

	return 0;
}

int main(int argc, char* argv[])
{
	parse_args(argc, argv);

	HuffmanCoder c;
	std::string lyrics = "Get your motor runnin'\n"
"Head out on the highway\n"
"Looking for adventure\n"
"In whatever comes our way\n"
"Yeah, darlin' gonna make it happen\n"
"Take the world in a love embrace\n"
"Fire all of your guns at once\n"
"And explode into space\n"
"I like smoke and lightnin'\n"
"Heavy metal thunder\n"
"Racing with the wind\n"
"And the feeling that I'm under\n"
"Yeah, darlin' gonna make it happen\n"
"Take the world in a love embrace\n"
"Fire all of your guns at once\n"
"And explode into space\n"
"Like a true nature's child\n"
"We were born\n"
"Born to be wild\n"
"We can climb so high\n"
"I never wanna die\n"
"Born to be wild\n"
"Born to be wild\n"
"Get your motor runnin'\n"
"Head out on the highway\n"
"Looking for adventure\n"
"In whatever comes our way\n"
"Yeah, darlin' gonna make it happen\n"
"Take the world in a love embrace\n"
"Fire all of your guns at once\n"
"And explode into space\n"
"Like a true nature's child\n"
"We were born\n"
"Born to be wild\n"
"We can climb so high\n"
"I never wanna die\n"
"Born to be wild\n"
"Born to be wild";

	c.encode(lyrics);
}
