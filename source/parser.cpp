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
				/*target parameter*/
				switch (t.get_type()){
					case Identifier:
						//TODO add register target
					case Number:
						if (t.get_numerical_value() < 0 || t.get_numerical_value() > 1) {
							//TODO error
							return false;
						}
						add_command->set_target(t.get_numerical_value());
					default:
						//TODO error;
						return false;
				}
				Lexer::nextToken(next, t, &next);
				/*next command parameter*/
				switch (t.get_type()){
					case Identifier:
						//TODO add register target
					case Number:
						if (t.get_numerical_value() < sub.first_line || t.get_numerical_value() > sub.last_line) {
							//TODO error
							return false;
						}
						add_command->set_jump(t.get_numerical_value());
					case Newline:
						if (sub.current_line == sub.last_line) {
							//TODO error
							return false;
						}
						add_command->set_jump(sub.current_line+1);
					default:
						//TODO error;
						return false;
				}
				command = add_command;
			}
			break;
		case Msub:
			{
				Msub_command* sub_command = new Msub_command();
				command = sub_command;
			}
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

