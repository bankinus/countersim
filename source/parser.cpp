#include "parser.h"
#include "token.h"
#include "lexer.h"
#include "simulator_command.h"
#include "context.h"
#include <string>
#include "error_stream.h"

namespace Parser {
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
		int i;
		next = s;
		Context * context = new Context();
		Simulator_command *command;
		/*parse routine header*/
		Lexer::nextToken(next, t, &next);
		switch (t.get_type()) {
			case Token::Def:
				break;
			default:
				error_stream << "syntax error in line " << context->current_line << ": "
					"expected def, received" << t.get_content() << Error_stream::endl;
				goto error_parse_Minsky_routine;
		}
		/*parse name*/
		switch (t.get_type()) {
			case Token::Identifier:
				context->set_name(t.get_content());
				break;
			default:
				error_stream << "syntax error in line " << context->current_line << ": "
					"expected identifier, received" << t.get_content() << Error_stream::endl;
				goto error_parse_Minsky_routine;
		}
		/*parse registers*/
		i = -1;
		for (Lexer::nextToken(next, t, &next);t.get_type()==Token::Identifier; Lexer::nextToken(next, t, &next)) {
			context->set_reg(t.get_content(), i);
			i--;
		}
		switch (t.get_type()) {
			case Token::BracketL:
				break;
			default:
				error_stream << "syntax error in line " << context->current_line << ": "
					"expected [, received" << t.get_content() << Error_stream::endl;
				goto error_parse_Minsky_routine;
		}
		/*parse exits*/
		i = -1;
		for (Lexer::nextToken(next, t, &next);t.get_type()==Token::Identifier; Lexer::nextToken(next, t, &next)) {
			context->set_line(t.get_content(), i);
			i--;
		}
		switch (t.get_type()) {
			case Token::BracketR:
				break;
			default:
				error_stream << "syntax error in line " << context->current_line << ": "
					"expected ], received" << t.get_content() << Error_stream::endl;
				goto error_parse_Minsky_routine;
		}
		Lexer::nextToken(next, t, &next);
		switch (t.get_type()) {
			case Token::Newline:
				break;
			default:
				error_stream << "syntax error in line " << context->current_line << ": "
					"expected newline, received" << t.get_content() << Error_stream::endl;
				goto error_parse_Minsky_routine;
		}
		while (1) {
			/*check for and parse label*/
			old = next;
			Lexer::nextToken(next, t, &next);
			if (t.get_type()==Token::EOP) break;
			switch (t.get_type()) {
				case Token::Identifier:
					context->set_line(t.get_content(), context->current_line);
					Lexer::nextToken(next, t, &next);
					switch (t.get_type()) {
						case Token::Colon:
							break;
						default:
							Lexer::nextToken(old, t, &next);
							error_stream << "syntax error in line " << context->current_line << ": "
								<< t.get_content() << " is not a valid command or label declaration" << Error_stream::endl;
							goto error_parse_Minsky_routine;
					}
					break;
				case Token::Madd:
				case Token::Msub:
					next = old;
					break;
				case Token::Call:
					//TODO insert subroutine
					break;
				default:
					Lexer::nextToken(old, t, &next);
					error_stream << "syntax error in line " << context->current_line << ": "
						<< t.get_content() << " is not a valid command or label declaration" << Error_stream::endl;
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

	bool parse_Minsky_command(const char *s, Simulator_command **res, Context &con) {
		Token t;
		const char *next;
		const char *old;
		next = s;
		Simulator_command *command = NULL;

		Lexer::nextToken(next, t, &next);
		switch (t.get_type()) {
			case Token::nil:
				error_stream << "lexing error in line " << con.current_line << Error_stream::endl;
				return false;
			case Token::Madd:
				{
					Madd_command *add_command = new Madd_command();
					Lexer::nextToken(next, t, &next);
					/*target parameter*/
					switch (t.get_type()){
						case Token::nil:
							error_stream << "lexing error in line " << con.current_line << Error_stream::endl;
							return false;
						case Token::Identifier:
							add_command->set_target(t.get_content());
							break;
						case Token::Number:
							if (t.get_numerical_value() > 1) {
								error_stream << "error in line " << con.current_line << ": register "
									<< t.get_content() << " is out of range" << Error_stream::endl;
								return false;
							}
							add_command->set_target(t.get_numerical_value());
							break;
						default:
							error_stream << "syntax error in line " << con.current_line << ": "
								<< t.get_content() << " is not a valid argument for add" << Error_stream::endl;
							return false;
					}
					old = next;
					Lexer::nextToken(next, t, &next);
					/*next command parameter*/
					switch (t.get_type()){
						case Token::nil:
							error_stream << "lexing error in line " << con.current_line << Error_stream::endl;
							return false;
						case Token::Identifier:
							add_command->set_jump(t.get_content());
							break;
						case Token::Number:
							add_command->set_jump(t.get_numerical_value());
							break;
						case Token::Newline:
							add_command->set_jump("_next");
							next = old;
							break;
						default:
							error_stream << "syntax error in line " << con.current_line << ": "
								<< t.get_content() << " is not a valid argument for add" << Error_stream::endl;
							return false;
					}
					Lexer::nextToken(next, t, &next);
					/*newline*/
					switch (t.get_type()){
						case Token::Newline:
							break;
						default:
							error_stream << "syntax error in line " << con.current_line << ": expected newline received"
								<< t.get_content() << Error_stream::endl;
							return false;
					}
					command = add_command;
				}
				break;
			case Token::Msub:
				{
					Msub_command* sub_command = new Msub_command();
					Lexer::nextToken(next, t, &next);
					/*target parameter*/
					switch (t.get_type()){
						case Token::nil:
							error_stream << "lexing error in line " << con.current_line << Error_stream::endl;
							return false;
						case Token::Identifier:
							sub_command->set_target(t.get_content());
							break;
						case Token::Number:
							if (t.get_numerical_value() < 0 || t.get_numerical_value() > 1) {
								error_stream << "error in line " << con.current_line << ": register "
									<< t.get_content() << " is out of range" << Error_stream::endl;
								return false;
							}
							sub_command->set_target(t.get_numerical_value());
							break;
						default:
							error_stream << "syntax error in line " << con.current_line << ": "
								<< t.get_content() << " is not a valid argument for sub" << Error_stream::endl;
							return false;
					}
					Lexer::nextToken(next, t, &next);
					/*jump or branch command parameter*/
					switch (t.get_type()){
						case Token::nil:
							error_stream << "lexing error in line " << con.current_line << Error_stream::endl;
							return false;
						case Token::Identifier:
							sub_command->set_jump(t.get_content());
							sub_command->set_branch(t.get_content());//set in case next token is newline
							break;
						case Token::Number:
							sub_command->set_jump(t.get_numerical_value());
							sub_command->set_branch(t.get_numerical_value());//set in case next token is newline
							break;
						default:
							error_stream << "syntax error in line " << con.current_line << ": "
								<< t.get_content() << " is not a valid argument for sub" << Error_stream::endl;
							return false;
					}
					old = next;
					Lexer::nextToken(next, t, &next);
					/*branch command parameter*/
					switch (t.get_type()){
						case Token::nil:
							error_stream << "lexing error in line " << con.current_line << Error_stream::endl;
							return false;
						case Token::Identifier:
							sub_command->set_branch(t.get_content());
							break;
						case Token::Number:
							sub_command->set_branch(t.get_numerical_value());
							break;
						case Token::Newline:
							sub_command->set_jump("_next");
							next = old;
							break;
						default:
							error_stream << "syntax error in line " << con.current_line << ": "
								<< t.get_content() << " is not a valid argument for sub" << Error_stream::endl;
							return false;
					}
					Lexer::nextToken(next, t, &next);
					/*newline*/
					switch (t.get_type()){
						case Token::Newline:
							break;
						default:
							error_stream << "syntax error in line " << con.current_line << ": expected newline received"
								<< t.get_content() << Error_stream::endl;
							return false;
					}
					command = sub_command;
				}
				break;
			default:
				error_stream << "syntax error in line " << con.current_line << ": expected minsky machine command, recieved"
					<< t.get_content() << Error_stream::endl;
				break;
		}
		if (command!=NULL) {
			command->set_line(con.current_line);
			*res=command;
			return true;
		}
		return false;
	}

	bool parse_URM_command(const char *s, Simulator_command** res, Context &con) {
		return false;
	}
}

