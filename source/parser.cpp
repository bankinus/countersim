#include "parser.h"
#include "error_stream.h"
#include "subroutine_inserter.h"
#include "routine_name_resolver.h"
#include "debug.h"

#include <string>
#include <sstream>
#include <algorithm>

std::string expectation(std::vector<Token::tokenType> exp) {
	std::stringstream ss;
	for (Token::tokenType t : exp) {
		ss << " " << Token::typeName(t);
	}
	return ss.str();
}

void Parser::unexpected_token(Token &t, std::vector<Token::tokenType> exp, std::vector<Token::tokenType> con) {
	//print error
	error_stream << "syntax error in line " << line << ": "
		"expected " << expectation(exp) << ", received " << t.get_content() << Error_stream::endl;
	//find continuation point
	if (con.empty()) return;
	while (t.get_type()!=Token::EOP && std::find(con.begin(), con.end(), t.get_type()) != con.end()) {
		lexer.getNext(t);
	}
}

Context *Parser::parse_simulator_program(const char *s) {
	Token t;
	line = 1;
	lexer.setNext(s);
	if (!swallow(Token::Preproc)) return NULL;
	if (!swallow(Token::Machine)) return NULL;
	lexer.getNext(t);
	switch (t.get_type()) {
		case Token::LRM:
			lexer.getNext(t);
			if (!(t.get_type()==Token::Number)) {
				unexpected_token(t, {Token::Number});
			}
			else {
				//TODO set limit
			}
		case Token::URM:
			return parse_program<Token::URM>();
			break;
		case Token::SRM:
			return parse_program<Token::SRM>();
			break;
		case Token::Minsky:
			return parse_program<Token::Minsky>();
			break;
		default:
			unexpected_token(t, {Token::LRM, Token::URM, Token::SRM, Token::Minsky});
			return NULL;
	}
}

template <Token::tokenType T> Context *Parser::parse_program() {
	Token t;
	bool subroutines = false;
	if (!swallow(Token::Newline)) return NULL;
	line++;
	for (lexer.peekNext(t); t.get_type()==Token::Preproc; lexer.peekNext(t)) {
		if (!parse_config_command()) return NULL;
	}
	for (lexer.peekNext(t); t.get_type()==Token::Def; lexer.peekNext(t)) {
		Context *subroutine_context;
		subroutine_context = parse_subroutine<T>();
		if (subroutine_context==NULL) {
			return NULL;
		}
		context_map[subroutine_context->get_name()] = subroutine_context;
		subroutines = true;
	}
	if  (subroutines) {
		if (!swallow(Token::Main)) {
			return NULL;
		}
		if (!swallow(Token::Newline)) return NULL;
		line++;
	}
	return parse_mainroutine<T>();
}

template <Token::tokenType T> Context *Parser::parse_mainroutine() {
	Context *context = new Context();
   context->set_name("main");
	return parse_routine<T>(*context);
}

template <Token::tokenType T> Context *Parser::parse_subroutine() {
	Token t;
	int i;
	Context *context = new Context();
	if (!swallow(Token::Def)) return NULL;
	lexer.getNext(t);
	if (t.get_type()!=Token::Identifier) {
		unexpected_token(t, {Token::Identifier});
		return NULL;
	}
	context->set_name(t.get_content());
	/*parse registers*/
	i = -1;
	for (lexer.getNext(t);t.get_type()==Token::Identifier; lexer.getNext(t)) {
		context->set_reg(t.get_content(), i);
		i--;
	}
	if (t.get_type() != Token::BracketL) {
		unexpected_token(t, {Token::BracketL});
		return NULL;
	}
	/*parse exits*/
	i = -1;
	for (lexer.getNext(t);t.get_type()==Token::Identifier; lexer.getNext(t)) {
		context->set_line(t.get_content(), i);
		i--;
	}
	if (t.get_type() != Token::BracketR) {
		unexpected_token(t, {Token::BracketR});
		return NULL;
	}
	if (!swallow(Token::Newline)) return NULL;
	line++;
	return parse_routine<T>(*context);
}

template <Token::tokenType T> Context *Parser::parse_routine(Context &context) {
	Token t;
	bool fail;
	while (1) {
		lexer.peekNext(t);
		if (t.get_type()==Token::EOP) break;
		if (t.get_type()==Token::Def) break;
		if (t.get_type()==Token::Main) break;
		if (t.get_type()==Token::Identifier) {
			context.set_line(t.get_content(), context.current_line);
			lexer.getNext(t);
			if (!swallow(Token::Colon, {Token::Newline})) {
				fail = true;
			}
		}
		if (!parse_instruction<T>(context)) fail=true;
		line++;
	}
	if (fail) {
		delete &context;
		return NULL;
	}
	Routine_name_resolver(context).visitc(context);
	return &context;
}

template <> bool Parser::parse_instruction<Token::Minsky>(Context &context) {
	Token t;
	lexer.getNext(t);
	Simulator_command *command = NULL;
	switch (t.get_type()) {
		case Token::Call:
			if (parse_call(context)) return true;
			return false;
		case Token::Madd:
			{
				Madd_command *add_command = new Madd_command();
				lexer.getNext(t);
				/*target parameter*/
				switch (t.get_type()){
					case Token::nil:
						error_stream << "lexing error in line " << line << Error_stream::endl;
						delete add_command;
						return false;
					case Token::Identifier:
						add_command->set_target_name(t.get_content());
						break;
					case Token::Number:
						if (t.get_numerical_value() > 1) {
							error_stream << "error in line " << line << ": register "
								<< t.get_content() << " is out of range" << Error_stream::endl;
							delete add_command;
							return false;
						}
						add_command->set_target(t.get_numerical_value());
						break;
					default:
						unexpected_token(t, {Token::Identifier, Token::Number}, {Token::Newline});
						delete add_command;
						return false;
				}
				lexer.getNext(t);
				/*next command parameter*/
				switch (t.get_type()){
					case Token::nil:
						error_stream << "lexing error in line " << line << Error_stream::endl;
						delete add_command;
						return false;
					case Token::Exit0:
						add_command->set_jump(0);
						break;
					case Token::Identifier:
						add_command->set_jump_name(t.get_content());
						break;
					case Token::Number:
						add_command->set_jump(t.get_numerical_value());
						break;
					case Token::Newline:
						add_command->set_jump_name("_next");
						break;
					default:
						unexpected_token(t, {Token::Identifier, Token::Number}, {Token::Newline});
						delete add_command;
						return false;
				}
				command = add_command;
			}
			break;
		case Token::Msub:
			{
				Msub_command* sub_command = new Msub_command();
				lexer.getNext(t);
				/*target parameter*/
				switch (t.get_type()){
					case Token::nil:
						error_stream << "lexing error in line " << line << Error_stream::endl;
						delete sub_command;
						return false;
					case Token::Identifier:
						sub_command->set_target_name(t.get_content());
						break;
					case Token::Number:
						if (t.get_numerical_value() < 0 || t.get_numerical_value() > 1) {
							error_stream << "error in line " << line << ": register "
								<< t.get_content() << " is out of range" << Error_stream::endl;
							delete sub_command;
							return false;
						}
						sub_command->set_target(t.get_numerical_value());
						break;
					default:
						unexpected_token(t, {Token::Identifier, Token::Number}, {Token::Newline});
						delete sub_command;
						return false;
				}
				lexer.getNext(t);
				/*jump or branch command parameter*/
				switch (t.get_type()){
					case Token::nil:
						error_stream << "lexing error in line " << line << Error_stream::endl;
						delete sub_command;
						return false;
					case Token::Exit0:
						sub_command->set_jump(0);
						sub_command->set_branch(0);//set in case next token is newline
						break;
					case Token::Identifier:
						sub_command->set_jump_name(t.get_content());
						sub_command->set_branch_name(t.get_content());//set in case next token is newline
						break;
					case Token::Number:
						sub_command->set_jump(t.get_numerical_value());
						sub_command->set_branch(t.get_numerical_value());//set in case next token is newline
						break;
					default:
						unexpected_token(t, {Token::Identifier, Token::Number}, {Token::Newline});
						delete sub_command;
						return false;
				}
				lexer.getNext(t);
				/*branch command parameter*/
				switch (t.get_type()){
					case Token::nil:
						error_stream << "lexing error in line " << line << Error_stream::endl;
						delete sub_command;
						return false;
					case Token::Exit0:
						sub_command->set_branch(0);
						break;
					case Token::Identifier:
						sub_command->set_branch_name(t.get_content());
						break;
					case Token::Number:
						sub_command->set_branch(t.get_numerical_value());
						break;
					case Token::Newline:
						sub_command->set_jump_name("_next");
						break;
					default:
						unexpected_token(t, {Token::Identifier, Token::Number}, {Token::Newline});
						delete sub_command;
						return false;
				}
				command = sub_command;
			}
			break;
		default:
			unexpected_token(t, {Token::Call, Token::Madd, Token::Msub});
			return false;
	}
	if (t.get_type()!=Token::Newline)
		lexer.getNext(t);
	/*newline*/
	switch (t.get_type()){
		case Token::Newline:
			break;
		default:
			unexpected_token(t, {Token::Newline}, {Token::Newline});
			if (command!=NULL)
				delete command;
			return false;
	}
	if (command!=NULL) {
		command->set_line(context.current_line);
		context.current_line++;
		command->set_actual_line(line);
		context.add_command(command);
		return true;
	}
	return false;
}

template <> bool Parser::parse_instruction<Token::SRM>(Context &context) {
	return false;
}

template <> bool Parser::parse_instruction<Token::URM>(Context &context) {
	return false;
}

bool Parser::parse_call(Context &context) {
	Token t;
	std::vector<int> registers;
	std::vector<int> exits;
	std::vector<std::string> exit_names;
	std::string routine_name;
	Context *subroutine;
	/*parse name*/
	lexer.getNext(t);
	switch (t.get_type()) {
		case Token::Identifier:
			routine_name = t.get_content();
			subroutine=context_map[routine_name];
			if (subroutine==NULL) {
				error_stream << "error in line " << line << ": "
				" undefined reference to " << routine_name << Error_stream::endl;
				return false;
			}
			break;
		default:
			unexpected_token(t, {Token::Newline});
			return false;
	}
	/*parse registers*/
	//TODO fix number of args
	registers=std::vector<int>();
	registers.push_back(0);
	for (lexer.getNext(t);t.get_type()!=Token::BracketL; lexer.getNext(t)) {
		switch (t.get_type()) {
			case Token::Number:
				registers.push_back(t.get_numerical_value());
				break;
			case Token::Identifier:
				registers.push_back(context.get_reg(t.get_content()));
				break;
			default:
				unexpected_token(t, {Token::Identifier, Token::Number}, {Token::Newline});
				return false;
		}
	}
	/*parse exits*/
	//TODO fix number of args
	exits=std::vector<int>();
	exit_names=std::vector<std::string>();
	exits.push_back(0);
	exit_names.push_back("");
	for (lexer.getNext(t);t.get_type()!=Token::BracketR; lexer.getNext(t)) {
		switch (t.get_type()) {
			case Token::Exit0:
				exits.push_back(0);
				exit_names.push_back("");
				break;
			case Token::Number:
				exits.push_back(t.get_numerical_value());
				exit_names.push_back("");
				break;
			case Token::Identifier:
				exits.push_back(0);
				exit_names.push_back(t.get_content());
				break;
			default:
				unexpected_token(t, {Token::Identifier, Token::Number}, {Token::Newline});
				return false;
		}
	}
	if (!swallow(Token::Newline, {Token::Newline})) return false;
	Subroutine_inserter(context, registers, exits, exit_names).visitc(*subroutine);
	return true;
}

bool Parser::parse_config_command() {
	Token t;
	if (!swallow(Token::Preproc)) return false;
	lexer.getNext(t);
	switch (t.get_type()) {
		case Token::Setreg:
			size_t reg;
			lexer.getNext(t);
			switch (t.get_type()) {
				case Token::Number:
					reg = t.get_numerical_value();
					break;
				default:
					unexpected_token(t, {Token::Number}, {Token::Newline});
					return false;
			}
			lexer.getNext(t);
			switch (t.get_type()) {
				case Token::Number:
					//TODO set reg
					break;
				default:
					unexpected_token(t, {Token::Number}, {Token::Newline});
					return false;
			}
			break;
		default:
			unexpected_token(t, {Token::Setreg}, {Token::Newline});
			return false;
	}
	if (!swallow(Token::Newline, {Token::Newline})) return false;
	line ++;
	return true;
}

bool Parser::swallow(Token::tokenType type, std::vector<Token::tokenType> con) {
	Token t;
	lexer.getNext(t);
	if (t.get_type()==type) return true;
	unexpected_token(t, {type}, con);
	return false;
}

