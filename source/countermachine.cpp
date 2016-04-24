#include "lexer.h"
#include "debug.h"
#include "simulation.h"
#include "context.h"
#include "parser.h"
#include "error_stream.h"
#include "gui/window.h"
#include "execution_visitor.h"
#include <string>
#include <iostream>
#include <fstream>
#include <QCommandLineParser>


/* This is file contains the main function for the simulator
 */

int main (int argc, char ** argv) {
	std::string inputfile("");
	std::string registerfile("");
	//Declare the supported options.
	QCommandLineParser parser;
	parser.addPositionalArgument("input-file", "path to file containing the program", "[input-file]");
	parser.addPositionalArgument("register-file", "path to file containing the register inputs", "[register-file]");
	parser.addOption(QCommandLineOption(QStringList() << "i" << "input-file",
		"path to file containing the program", "input-file"));
	parser.addOption(QCommandLineOption(QStringList() << "f" << "register-file",
		"path to file containing input register", "register-file"));
	parser.addOption(QCommandLineOption(QStringList() << "g" << "gui",
		"if this flag is set program simulator runs in graphical mode"));
	parser.addOption(QCommandLineOption(QStringList() << "r" << "run",
		"if this flag is set program simulator runs program to completion upon start"));
	parser.addVersionOption();
	parser.addHelpOption();
	parser.process(QCoreApplication(argc, argv));

	if (parser.isSet("input-file")) {
		inputfile = parser.value("input-file").toStdString();
	} else if (parser.positionalArguments().size() > 0) {
		inputfile = parser.positionalArguments()[0].toStdString();
	} else {
		if (!parser.isSet("gui")) {
			std::cerr << "error: no inputfile specified in non graphical mode\n";
			return 0;
		}
	}
	if (parser.isSet("register-file")) {
		registerfile = parser.value("register-file").toStdString();
	} else if (parser.positionalArguments().size() > 1) {
		registerfile = parser.positionalArguments()[1].toStdString();
	}

	if (parser.isSet("gui")) {
		//run in console mode
		error_stream.set_console(false);
		return graphical_execution(argc, argv, parser);
	}
	else {
		//run in console mode
		Simulation simulation;
		std::string program;
		//load program
		std::ifstream filestream(inputfile);
		if (!filestream.is_open()) {
			std::cerr << "error: could not open file:" << inputfile << "\n";
			return 0;
		}
		std::stringstream bufstream;
		bufstream << filestream.rdbuf();
		program = bufstream.str();
		//parse program
		Context *context = Parser().parse_simulator_program(program.c_str());
		if (context==NULL) return 0;
		//enter registers
		if (registerfile != "") {
			std::ifstream reg_in_stream(registerfile);
			if (!reg_in_stream.is_open()) {
				std::cerr << "error: could not open file:" << registerfile << "\n";
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
			std::cout << "enter registers (end register input by sending ctr-d):" << std::endl;
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
		if (parser.isSet("run")) {
			exe.visitc(*context);
		}
		else {
			std::cin.clear();
			bool done = false;
			while (!done && !std::cin.eof()) {
				char cmd;
				std::cout << "enter command (h for help):" << std::endl;
				std::cin >> cmd;
				switch (cmd) {
					case 'h':
						std::cout << "available commands are:" << std::endl;
						std::cout << "\th: display commands" << std::endl;
						std::cout << "\ts: single step" << std::endl;
						std::cout << "\tr: run program to completion" << std::endl;
						std::cout << "\tq: quit" << std::endl;
						break;
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
