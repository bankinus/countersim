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

Context *parse_URM_routine(const char *s) {
	Context * context = new Context();
	return context;
}

Context *parse_Minsky_routine(const char *s) {
	Token t;
	const char *old;
	const char *next;
	next = s;
	Context * context = new Context();
	Simulator_command *command;
	/*parse routine header*/
	//TODO
	while (1) {
		/*check for label*/
		old = next;
		Lexer::nextToken(next, t, &next);
		if (t.get_type()==EOP) break;
		switch (t.get_type()) {
			case Identifier:
				context->set_line(t.get_content(), context->current_line);
				Lexer::nextToken(next, t, &next);
				switch (t.get_type()) {
					case Colon:
						break;
					default: 
						//TODO error
						goto error_parse_Minsky_routine;
				}
				break;
			case Madd:
			case Msub:
				next = old;
				break;
			default:
				//TODO error
				goto error_parse_Minsky_routine;
		}
		/*parse command*/
		if (!parse_Minsky_command(next, &command, *context)){
			goto error_parse_Minsky_routine;
		}
	}
	/*replace labels*/
	//TODO
	return context;
	error_parse_Minsky_routine:
		delete context;
		return NULL;
}

bool parse_Minsky_command(const char *s, Simulator_command **res, Context &sub) {
	Token t;
	const char *next;
	const char *old;
	next = s;
	Simulator_command *command = NULL;

	Lexer::nextToken(next, t, &next);
	switch (t.get_type()) {
		case nil:
			std::cerr << "lexing error in line " << sub.current_line << std::endl;
			return false;
		case Madd:
			{
				Madd_command *add_command = new Madd_command();
				Lexer::nextToken(next, t, &next);
				/*target parameter*/
				switch (t.get_type()){
					case nil:
						std::cerr << "lexing error in line " << sub.current_line << std::endl;
						return false;
					case Identifier:
						add_command->set_target(t.get_content());
						break;
					case Number:
						if (t.get_numerical_value() > 1) {
							std::cerr << "error in line " << sub.current_line << ": register " << t.get_content() << " is out of range" << std::endl;
							return false;
						}
						add_command->set_target(t.get_numerical_value());
						break;
					default:
						std::cerr << "syntax error: " << t.get_content() << " is not a valid argument for add" << std::endl;
						return false;
				}
				old = next;
				Lexer::nextToken(next, t, &next);
				/*next command parameter*/
				switch (t.get_type()){
					case nil:
						std::cerr << "lexing error in line " << sub.current_line << std::endl;
						return false;
					case Identifier:
						add_command->set_jump(t.get_content());
						break;
					case Number:
						add_command->set_jump(t.get_numerical_value());
						break;
					case Newline:
						add_command->set_jump("_next");
						next = old;
						break;
					default:
						std::cerr << "syntax error: " << t.get_content() << " is not a valid argument for add" << std::endl;
						return false;
				}
				Lexer::nextToken(next, t, &next);
				/*newline*/
				switch (t.get_type()){
					case Newline:
						break;
					default:
						std::cerr << "syntax error in line " << sub.current_line << ": expected newline received" << t.get_content() << std::endl;
						return false;
				}
				command = add_command;
			}
			break;
		case Msub:
			{
				Msub_command* sub_command = new Msub_command();
				Lexer::nextToken(next, t, &next);
				/*target parameter*/
				switch (t.get_type()){
					case nil:
						std::cerr << "lexing error in line " << sub.current_line << std::endl;
						return false;
					case Identifier:
						sub_command->set_target(t.get_content());
						break;
					case Number:
						if (t.get_numerical_value() < 0 || t.get_numerical_value() > 1) {
							std::cerr << "error in line " << sub.current_line << ": register " << t.get_content() << " is out of range" << std::endl;
							return false;
						}
						sub_command->set_target(t.get_numerical_value());
						break;
					default:
						std::cerr << "syntax error: " << t.get_content() << " is not a valid argument for sub" << std::endl;
						return false;
				}
				Lexer::nextToken(next, t, &next);
				/*jump or branch command parameter*/
				switch (t.get_type()){
					case nil:
						std::cerr << "lexing error in line " << sub.current_line << std::endl;
						return false;
					case Identifier:
						sub_command->set_jump(t.get_content());
						sub_command->set_branch(t.get_content());//set in case next token is newline
						break;
					case Number:
						sub_command->set_jump(t.get_numerical_value());
						sub_command->set_branch(t.get_numerical_value());//set in case next token is newline
						break;
					default:
						std::cerr << "syntax error: " << t.get_content() << " is not a valid argument for sub" << std::endl;
						return false;
				}
				old = next;
				Lexer::nextToken(next, t, &next);
				/*branch command parameter*/
				switch (t.get_type()){
					case nil:
						std::cerr << "lexing error in line " << sub.current_line << std::endl;
						return false;
					case Identifier:
						sub_command->set_branch(t.get_content());
						break;
					case Number:
						sub_command->set_branch(t.get_numerical_value());
						break;
					case Newline:
						sub_command->set_jump("_next");
						next = old;
						break;
					default:
						std::cerr << "syntax error: " << t.get_content() << " is not a valid argument for sub" << std::endl;
						return false;
				}
				Lexer::nextToken(next, t, &next);
				/*newline*/
				switch (t.get_type()){
					case Newline:
						break;
					default:
						std::cerr << "syntax error in line " << sub.current_line << ": expected newline received" << t.get_content() << std::endl;
						return false;
				}
				command = sub_command;
			}
			break;
		default:
			std::cerr << "syntax error: expected minsky machine command, recieved" << t.get_content() << std::endl;
			break;
	}
	if (command!=NULL) {
		command->set_line(sub.current_line);
		*res=command;
		return true;
	}
	return false;
}

bool parse_URM_command(const char *s, Simulator_command** res, Context &sub) {
	return false;
}

