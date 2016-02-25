#include "parser.h"
#include "token.h"
#include "lexer.h"
#include "simulator_command.h"
#include "context.h"
#include "routine_name_resolver.h"
#include <string>
#include "error_stream.h"

namespace Parser {
	Context *parse_simulator_program(const char *s) {
		Token t;
		const char *old;
		const char *next;
		std::vector<Context*> routines;
		Context *context;
		next = s;
		context = new Context();
		context->current_line = 1;
		old = next;
		Lexer::nextToken(next, t, &next);
		while (t.get_type()==Token::Preproc) {
			parse_config_command(next, context, &next);
			old = next;
			Lexer::nextToken(next, t, &next);
			context->current_line++;
		}
		return parse_Minsky_program(old);
	}

	bool parse_config_command(const char *s, Context *context, const char **resnext) {
		Token t;
		const char* next;
		Lexer::nextToken(next, t, &next);
		switch (t.get_type()) {
			case Token::Machine:
				Lexer::nextToken(next, t, &next);
				switch (t.get_type()) {
					case Token::Minsky:
						//TODO set config
						break;
					case Token::URM:
						//TODO set config
						break;
					case Token::LRM:
						//TODO set config
						break;
					default:
						error_stream << "syntax error in line " << context->current_line << ": "
							"expected machine type, received " << t.get_content() << Error_stream::endl;
						goto error_config_command;
				}
				break;
			case Token::Setreg:
				Lexer::nextToken(next, t, &next);
				switch (t.get_type()) {
					case Token::Number:
						//TODO set config
						break;
					default:
						error_stream << "syntax error in line " << context->current_line << ": "
							"expected integer, received " << t.get_content() << Error_stream::endl;
						goto error_config_command;
				}
				Lexer::nextToken(next, t, &next);
				switch (t.get_type()) {
					case Token::Number:
						//TODO set config
						break;
					default:
						error_stream << "syntax error in line " << context->current_line << ": "
							"expected integer, received " << t.get_content() << Error_stream::endl;
						goto error_config_command;
				}
				break;
			default:
				error_stream << "syntax error in line " << context->current_line << ": "
					"expected config command, received " << t.get_content() << Error_stream::endl;
				goto error_config_command;
		}
		Lexer::nextToken(next, t, &next);
		switch (t.get_type()) {
			case Token::Newline:
				break;
			default:
				error_stream << "syntax error in line " << context->current_line << ": "
					"expected newline, received " << t.get_content() << Error_stream::endl;
				goto error_config_command;
		}
		*resnext = next;
		return true;
		error_config_command:
			return false;
	}

	Context *parse_Minsky_program(const char *s) {
		Token t;
		const char *old;
		const char *next;
		std::vector<Context*> routines;
		Context *context;
		bool main;
		main = false;
		next = s;
		old = next;
		Lexer::nextToken(next, t, &next);
		while (!main) {
			switch (t.get_type()) {
				case Token::Def:
					context = parse_Minsky_sub_routine(old);
					if (context==NULL) goto error_parse_Minsky_program;
					routines.push_back(context);
					next = context->next;
					break;
				case Token::Identifier:
				case Token::Madd:
				case Token::Msub:
				case Token::EOP:
					next=old;
				case Token::Main:
					main = true;
					break;
				default:
					error_stream << "syntax error in line " << context->current_line << ": "
						"expected routine definition, received " << t.get_content() << Error_stream::endl;
					goto error_parse_Minsky_program;
			}
		}
		context = parse_Minsky_main_routine(next);
		if (context==NULL) goto error_parse_Minsky_program;
		/*cleanup*/
		for (auto c: routines) {
			delete c;
		}
		return context;

		error_parse_Minsky_program:
			for (auto c: routines) {
				delete c;
			}
			return NULL;
	}

	Context *parse_URM_program(const char *s) {
		return NULL;
	}

	Context *parse_URM_routine(const char *s) {
		return NULL;
	}

	Context *parse_Minsky_main_routine(const char *s) {
		Context *context = new Context();
		if (parse_Minsky_routine(s, context)!=NULL) {
			return context;
		}
		return NULL;
	}

	Context *parse_Minsky_sub_routine(const char *s) {
		Token t;
		const char *old;
		const char *next;
		int i;
		next = s;
		Context *context = new Context();
		Simulator_command *command;
		/*parse routine header*/
		Lexer::nextToken(next, t, &next);
		switch (t.get_type()) {
			case Token::Def:
				break;
			default:
				error_stream << "syntax error in line " << context->current_line << ": "
					"expected def, received " << t.get_content() << Error_stream::endl;
				goto error_parse_Minsky_routine;
		}
		/*parse name*/
		Lexer::nextToken(next, t, &next);
		switch (t.get_type()) {
			case Token::Identifier:
				context->set_name(t.get_content());
				break;
			default:
				error_stream << "syntax error in line " << context->current_line << ": "
					"expected identifier, received " << t.get_content() << Error_stream::endl;
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
					"expected [, received " << t.get_content() << Error_stream::endl;
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
					"expected ], received " << t.get_content() << Error_stream::endl;
				goto error_parse_Minsky_routine;
		}
		if (parse_Minsky_routine(s, context)!=NULL) {
			return context;
		}
		error_parse_Minsky_routine:
			return NULL;
	}

	Context *parse_Minsky_routine(const char *s, Context *context) {
		Token t;
		const char *old;
		const char *next;
		next = s;
		Simulator_command *command;
		while (1) {
			old = next;
			Lexer::nextToken(next, t, &next);
			/*check for end of routine*/
			if (t.get_type()==Token::EOP) break;
			if (t.get_type()==Token::Def) break;
			if (t.get_type()==Token::Main) break;
			/*check for and parse label*/
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
			else {
				context->add_command(command);
				context->current_line++;
			}
		}
		context->next=old;
		/*replace labels*/
		Routine_name_resolver(*context).visitc(*context);
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
							delete add_command;
							return false;
						case Token::Identifier:
							add_command->set_target_name(t.get_content());
							break;
						case Token::Number:
							if (t.get_numerical_value() > 1) {
								error_stream << "error in line " << con.current_line << ": register "
									<< t.get_content() << " is out of range" << Error_stream::endl;
								delete add_command;
								return false;
							}
							add_command->set_target(t.get_numerical_value());
							break;
						default:
							error_stream << "syntax error in line " << con.current_line << ": "
								<< t.get_content() << " is not a valid argument for add" << Error_stream::endl;
							delete add_command;
							return false;
					}
					old = next;
					Lexer::nextToken(next, t, &next);
					/*next command parameter*/
					switch (t.get_type()){
						case Token::nil:
							error_stream << "lexing error in line " << con.current_line << Error_stream::endl;
							delete add_command;
							return false;
						case Token::Identifier:
							add_command->set_jump_name(t.get_content());
							break;
						case Token::Number:
							add_command->set_jump(t.get_numerical_value());
							break;
						case Token::Newline:
							add_command->set_jump_name("_next");
							next = old;
							break;
						default:
							error_stream << "syntax error in line " << con.current_line << ": "
								<< t.get_content() << " is not a valid argument for add" << Error_stream::endl;
							delete add_command;
							return false;
					}
					Lexer::nextToken(next, t, &next);
					/*newline*/
					switch (t.get_type()){
						case Token::Newline:
							break;
						default:
							error_stream << "syntax error in line " << con.current_line << ": expected newline received "
								<< t.get_content() << Error_stream::endl;
							delete add_command;
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
							delete sub_command;
							return false;
						case Token::Identifier:
							sub_command->set_target_name(t.get_content());
							break;
						case Token::Number:
							if (t.get_numerical_value() < 0 || t.get_numerical_value() > 1) {
								error_stream << "error in line " << con.current_line << ": register "
									<< t.get_content() << " is out of range" << Error_stream::endl;
								delete sub_command;
								return false;
							}
							sub_command->set_target(t.get_numerical_value());
							break;
						default:
							error_stream << "syntax error in line " << con.current_line << ": "
								<< t.get_content() << " is not a valid argument for sub" << Error_stream::endl;
							delete sub_command;
							return false;
					}
					Lexer::nextToken(next, t, &next);
					/*jump or branch command parameter*/
					switch (t.get_type()){
						case Token::nil:
							error_stream << "lexing error in line " << con.current_line << Error_stream::endl;
							delete sub_command;
							return false;
						case Token::Identifier:
							sub_command->set_jump_name(t.get_content());
							sub_command->set_branch_name(t.get_content());//set in case next token is newline
							break;
						case Token::Number:
							sub_command->set_jump(t.get_numerical_value());
							sub_command->set_branch(t.get_numerical_value());//set in case next token is newline
							break;
						default:
							error_stream << "syntax error in line " << con.current_line << ": "
								<< t.get_content() << " is not a valid argument for sub" << Error_stream::endl;
							delete sub_command;
							return false;
					}
					old = next;
					Lexer::nextToken(next, t, &next);
					/*branch command parameter*/
					switch (t.get_type()){
						case Token::nil:
							error_stream << "lexing error in line " << con.current_line << Error_stream::endl;
							delete sub_command;
							return false;
						case Token::Identifier:
							sub_command->set_branch_name(t.get_content());
							break;
						case Token::Number:
							sub_command->set_branch(t.get_numerical_value());
							break;
						case Token::Newline:
							sub_command->set_jump_name("_next");
							next = old;
							break;
						default:
							error_stream << "syntax error in line " << con.current_line << ": "
								<< t.get_content() << " is not a valid argument for sub" << Error_stream::endl;
							delete sub_command;
							return false;
					}
					Lexer::nextToken(next, t, &next);
					/*newline*/
					switch (t.get_type()){
						case Token::Newline:
							break;
						default:
							error_stream << "syntax error in line " << con.current_line << ": expected newline received "
								<< t.get_content() << Error_stream::endl;
							delete sub_command;
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

