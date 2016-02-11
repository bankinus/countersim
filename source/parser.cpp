#include "parser.h"
#include "token.h"
#include "lexer.h"
#include "simulator_command.h"
#include "context.h"
#include <string>
#include <iostream>

bool parse_simulator_program(const char *s) {
	return false;
}

bool parse_config_command(const char *s) {
	return false;
}
bool parse_Minsky_program(const char *s) {
	return false;
}

bool parse_URM_program(const char *s) {
	return false;
}

bool parse_URM_routine(const char *s) {
	return false;
}

bool parse_Minsky_command(const char *s, Simulator_command **res, Context &sub) {
	Token t;
	const char *next;
	next = s;
	Lexer::nextToken(next, t, &next);
	Simulator_command *command = 0;
	switch (t.get_type()) {
		case Madd:
			{
				Madd_command *add_command = new Madd_command();
				Lexer::nextToken(next, t, &next);
				//TODO check for lexing error
				/*target parameter*/
				switch (t.get_type()){
					case Identifier:
						add_command->set_target(t.get_content());
					case Number:
						if (t.get_numerical_value() < 0 || t.get_numerical_value() > 1) {
							std::cerr << "error: register " << t.get_content() << " is out of range" << std::endl;
							return false;
						}
						add_command->set_target(t.get_numerical_value());
					default:
						std::cerr << "syntax error: " << t.get_content() << " is not a valid argument for add" << std::endl;
						return false;
				}
				Lexer::nextToken(next, t, &next);
				//TODO check for lexing error
				/*next command parameter*/
				switch (t.get_type()){
					case Identifier:
						add_command->set_jump(t.get_content());
					case Number:
						add_command->set_jump(t.get_numerical_value());
					case Newline:
						add_command->set_jump("_next");
					default:
						std::cerr << "syntax error: " << t.get_content() << " is not a valid argument for add" << std::endl;
						return false;
				}
				command = add_command;
			}
			break;
		case Msub:
			{
				Msub_command* sub_command = new Msub_command();
				//TODO parse args
				command = sub_command;
			}
			break;
		default:
			std::cerr << "syntax error: expected minsky machine command, recieved" << t.get_content() << std::endl;
			break;
	}
	if (command!=NULL) {
		command->set_line(sub.current_line);
		return true;
	}
	return false;
}

bool parse_URM_command(const char *s, Simulator_command** res, Context &sub) {
	return false;
}

