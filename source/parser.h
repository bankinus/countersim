#pragma once
#include "simulator_command.h"
#include "context.h"
#include "token.h"
#include "lexer.h"
#include <unordered_map>
#include <vector>

class Parser {
	protected:
		std::unordered_map<std::string, Context*> context_map;
		unsigned long long int line;
		Lexer lexer;
		std::vector<size_t> breakpoints;
	public:
		Parser () : context_map(), line(1), lexer(), breakpoints(){}
		Context *parse_simulator_program(const char *s);

	protected:
		template <Token::tokenType> Context *parse_program();
		template <Token::tokenType> Context *parse_mainroutine();
		template <Token::tokenType> Context *parse_subroutine();
		template <Token::tokenType> Context *parse_routine(Context &context);
		template <Token::tokenType> bool parse_instruction(Context &con);
		bool parse_call(Context &con);
		bool parse_config_command();
		bool swallow(Token::tokenType type, std::vector<Token::tokenType> con = {});
		void unexpected_token(Token &t, std::vector<Token::tokenType> exp={}, std::vector<Token::tokenType> con={});
};

