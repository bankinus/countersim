#include "debug.h"
#include "token.h"
#include "lexer.h"
#include "parser.h"
#include "context.h"
#include <stdio.h>
#include <iostream>

#ifndef DEBUG
template <typename T> DebugStream &DebugStream::operator<<(T) {
	return *this;
}
#endif

void _lexertest() {
	Token t;
	char s[5000];
	const char *pos;
	while (1) {
		if (fgets(s, 5000, stdin)==NULL) return;
		pos = s;
		while (1) {
			Lexer::nextToken(pos, t, &pos);
			std::cout << "<" << t.get_content() << ">";
			std::cout.flush();
			if (pos == NULL) {
				std::cout << std::endl;
				break;
			}
			if (t.get_type()==Token::Newline) break;
			if (t.get_type()==Token::EOP) break;
			if (t.get_type()==Token::nil) break;
		}
	}
	return;
}

void _parse_Minsky_test() {
	Simulator_command *command;
	Context con;
	const char *next;
	if (Parser::parse_Minsky_command("add foo 5\n", &next, &command, con)) {std::cout << command->toString() << std::endl; delete command;}
	else {std::cout << "bad parse" << std::endl;}
	if (Parser::parse_Minsky_command("sub 1 bar\n", &next, &command, con)) {std::cout << command->toString() << std::endl;delete command;}
	else {std::cout << "bad parse" << std::endl;}
}

void _parse_Minsky_program_test() {
	Simulator_command *command;
	Context con;
	char buf[2500000];
	char *c;
	int clast = ~EOF;
	c = buf;
	for (c=buf; clast!=EOF; c++) {
		clast = fgetc(stdin);
		*c = (char) clast;
	}
	*(c-1) = '\0';
	Parser::parse_simulator_program(buf);
}

