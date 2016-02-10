#pragma once
#include "token.h"
#include "lexer.h"
#include <stdio.h>
#include <iostream>

void lexertest() {
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
			if (t.get_type()==Newline) break;
			if (t.get_type()==EOP) break;
			if (t.get_type()==nil) break;
		}
	}
	return;
}

