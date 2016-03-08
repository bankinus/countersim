#include "lexer.h"
#include "debug.h"
#include <string>
#include <iostream>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

/* This is file contains the main function for the simulator
 */

int main (int argc, char ** argv) {
	//Declare the supported options.
	po::options_description description("Allowed options");
	description.add_options()
		("input-file", po::value<std::string>(), "path to file containing the program")
		("register-file", po::value<std::string>(), "path to file containing register inputs")
		("gui,g", "if this flag is set program simulator runs in graphical mode")
		("help,h", "produce help message")
		;
	
	po::positional_options_description pos;
	pos.add("input-file", 1);
	pos.add("register-file", 2);
	
	po::variables_map v_map;
	try {
	po::store(po::command_line_parser(argc, argv).
	          options(description).positional(pos).run(), v_map);
	} catch (po::error &e) {
		std::cerr << "error:" << e.what() << "\n";
		return 0;
	}
	po::notify(v_map);
	
	if (v_map.count("help")) {
		std::cout << description << "\n";
		return 1;
	}
	
	if (v_map.count("input-file")) {
		std::cout << "reading program from file: " 
		     << v_map["input-file"].as<std::string>() << "\n";
	} else {
		if (!v_map.count("gui")) {
			std::cerr << "error: no inputfile specified in non graphical mode\n";
		}
	}

	if (v_map.count("gui")) {
		std::cerr << "error: gui not yet implemented\n";
		return 0;
	}

	return 1;
}
