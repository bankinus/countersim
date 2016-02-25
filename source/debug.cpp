#include "debug.h"
#include "token.h"
#include "lexer.h"
#include "parser.h"
#include "context.h"
#include <stdio.h>
#include <iostream>

#ifndef DEBUG
DebugStream& DebugStream::operator<<(std::ostream& (*f)(std::ostream&))
{
    f(std::cout);
    return *this;
}
DebugStream debug;
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

void _parse_Minsky_program_test() {
	Simulator_command *command;
	Context *con;
	Parser parser;
	char buf[2500000];
	char *c;
	int clast = ~EOF;
	c = buf;
	for (c=buf; clast!=EOF; c++) {
		clast = fgetc(stdin);
		*c = (char) clast;
	}
	*(c-1) = '\0';
	con = parser.parse_simulator_program(buf);
	if (con==NULL) return;
	for (auto c: con->get_program()) {
		debug << c->toString() << std::endl;
	}
}

