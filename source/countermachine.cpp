#include "lexer.h"
#include "debug.h"
#include "simulation.h"
#include "context.h"
#include "parser.h"
#include "gui/window.h"
#include "execution_visitor.h"
#include <string>
#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

/* This is file contains the main function for the simulator
 */

int main (int argc, char ** argv) {
	//Declare the supported options.
	po::options_description description("useage: countermachine [program] [register-presets] [options]\nAllowed options");
	description.add_options()
		("input-file", po::value<std::string>(), "path to file containing the program")
		("register-file", po::value<std::string>(), "path to file containing register inputs")
		("gui,g", "if this flag is set program simulator runs in graphical mode")
		("run,r", "if this flag is set program simulator runs program to completion upon start")
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
			return 0;
		}
	}

	if (v_map.count("gui")) {
		//run in console mode
		return graphical_execution(argc, argv, v_map);
	}
	else {
		//run in console mode
		Simulation simulation;
		std::string program;
		//load program
		std::ifstream filestream(v_map["input-file"].as<std::string>());
		if (!filestream.is_open()) {
			std::cerr << "error: could not open file:" << v_map["input-file"].as<std::string>() << "\n";
			return 0;
		}
		std::stringstream bufstream;
		bufstream << filestream.rdbuf();
		program = bufstream.str();
		//parse program
		Context *context = Parser().parse_simulator_program(program.c_str());
		if (context==NULL) return 0;
		int i = 1;
		for (Simulator_command *cmd: context->get_program()) {
			debug << i << ": " << cmd->toString() << std::endl;
			i++;
		}
		//enter registers
		if (v_map.count("register-file")) {
			std::ifstream reg_in_stream(v_map["input-file"].as<std::string>());
			if (!reg_in_stream.is_open()) {
				std::cerr << "error: could not open file:" << v_map["input-file"].as<std::string>() << "\n";
				return 0;
			}
			for (size_t i=0; !reg_in_stream.eof(); i++){
				unsigned long long int val;
				reg_in_stream >> val;
				if (reg_in_stream.eof()) break;
				simulation.get_register(i)->set_value(val);
			}
		}
		else {
			for (size_t i=0; !std::cin.eof() && (i<simulation.get_max_reg() || simulation.get_max_reg()==0); i++){
				unsigned long long int val;
				if (!(std::cin >> val)) {
					if (std::cin.eof()) break;
					else {
						std::cerr << "error: input is not a number" << std::endl;
						return 0;
					}
				}
				simulation.get_register(i)->set_value(val);
			}
		}
		//run program
		Execution_visitor exe(simulation);
		if (v_map.count("run")) {
			exe.visitc(*context);
		}
		else {
			std::cin.clear();
			bool done;
			while (!done && !std::cin.eof()) {
				char cmd;
				std::cin >> cmd;
				switch (cmd) {
					case 's':
						done = !exe.step_visitc(*context);
						if (done) break;
						//print register content after step
						std::cout << "register content after step:" << std::endl;
						for (const Register &reg: simulation.get_registers()) {
							std::cout << reg.get_value() << std::endl;
						}
						std::cout << "next is line:" << exe.get_next() << std::endl;
						break;
					case 'r': 
						exe.visitc(*context);
						done = true;
						break;
					case 'q':
						return 0;
						break;
					default:
						std::cerr << "no valid command:" << cmd << std::endl;
						std::cin.clear();
						break;
				}
			}
		}
		//print registers at end of execution
		std::cout << "execution terminated with the following register contents:" << std::endl;
		for (const Register &reg: simulation.get_registers()) {
			std::cout << reg.get_value() << std::endl;
		}
		
		return 1;
	}

	return 0;
}
