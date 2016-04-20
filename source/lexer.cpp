#include "lexer.h"
#include <string>
#include <iostream>

/// skips characters in d
/**
 * nextToken
 *
 * lexes the string passed in s until it has read the next token
 * skips character in string d of length n
 * string needs terminating character that isn't skipped
 *
 * @param s string to skip through
 * @param d characters to skip
 */
const char* skip(const char *s, const char *d, int n){
	int i;
	while (1)
	{
		for (i = 0; i < n; i++) {
			if (d[i] == *s) break;
		}
		if (i<n) s++;
		else break;
	}
	return s;
}

bool isdelim(char c) {
	const char *delim = "\0 \t\n:#[]";
	const char *pos = delim;
	do {
		if (c==*pos) return true;
		pos++;
	} while (*pos!='\0');
	return false;
}

bool tokencmpr (const char *s, const char *t) {
	for (; *t!=0; s++,t++){
		if (*s!=*t) return false;
	}
	if (isdelim(*s)) return true;
	else return false;
}

const char * _nextToken(const char *s, class Token &t, bool next) {
	s = skip(s, " \t", 2);
	if (s[0]=='\0') {
		t.set_type(Token::EOP);
		t.set_content("EOP");
	}
	else if (s[0]=='\n') {
		t.set_type(Token::Newline);
		t.set_content("\n");
		s+=1;
	}
	else if (s[0]=='#') {
		t.set_type(Token::Preproc);
		t.set_content("#");
		s+=1;
	}
	else if (s[0]=='[') {
		t.set_type(Token::BracketL);
		t.set_content("[");
		s+=1;
	}
	else if (s[0]==']') {
		t.set_type(Token::BracketR);
		t.set_content("]");
		s+=1;
	}
	else if (s[0]==':') {
		t.set_type(Token::Colon);
		t.set_content(":");
		s+=1;
	}
	else if (*s>='0' && *s<='9') {
		long long number = 0;
		for (int i=0; ;i++) {
			if (s[i]>='0' && s[i]<='9') {
				number*=10;
				number+=s[i]-'0';
				t.add_content(s[i]);
			}
			else if (tokencmpr(s+i, "")) {
				t.set_type(Token::Number);
				t.set_numerical_value(number);
				s+=i;
				break;
			}
			else {
				t.set_content("lexing error");
				return NULL;
			}
		}
	}
	else if (tokencmpr(s, "inc")) {
		t.set_type(Token::Uinc);
		t.set_content("inc");
		s+=3;
	}
	else if (tokencmpr(s, "dec")) {
		t.set_type(Token::Udec);
		t.set_content("dec");
		s+=3;
	}
	else if (tokencmpr(s, "cpy")) {
		t.set_type(Token::Ucopy);
		t.set_content("cpy");
		s+=3;
	}
	else if (tokencmpr(s, "clr")) {
		t.set_type(Token::Uclear);
		t.set_content("clr");
		s+=3;
	}
	else if (tokencmpr(s, "jmp")) {
		t.set_type(Token::Ujump);
		t.set_content("jmp");
		s+=3;
	}
	else if (tokencmpr(s, "add")) {
		t.set_type(Token::Madd);
		t.set_content("add");
		s+=3;
	}
	else if (tokencmpr(s, "sub")) {
		t.set_type(Token::Msub);
		t.set_content("sub");
		s+=3;
	}
	else if (tokencmpr(s, "mul")) {
		t.set_type(Token::Mmul);
		t.set_content("mul");
		s+=3;
	}
	else if (tokencmpr(s, "div")) {
		t.set_type(Token::Mdiv);
		t.set_content("div");
		s+=3;
	}
	else if (tokencmpr(s, "def")) {
		t.set_type(Token::Def);
		t.set_content("def");
		s+=3;
	}
	else if (tokencmpr(s, "set")) {
		t.set_type(Token::Setreg);
		t.set_content("set");
		s+=3;
	}
	else if (tokencmpr(s, "srm")) {
		t.set_type(Token::SRM);
		t.set_content("srm");
		s+=3;
	}
	else if (tokencmpr(s, "lrm")) {
		t.set_type(Token::LRM);
		t.set_content("lrm");
		s+=3;
	}
	else if (tokencmpr(s, "urm")) {
		t.set_type(Token::URM);
		t.set_content("urm");
		s+=3;
	}
	else if (tokencmpr(s, "call")) {
		t.set_type(Token::Call);
		t.set_content("call");
		s+=4;
	}
	else if (tokencmpr(s, "main")) {
		t.set_type(Token::Main);
		t.set_content("main");
		s+=4;
	}
	else if (tokencmpr(s, "break")) {
		t.set_type(Token::Break);
		t.set_content("break");
		s+=5;
	}
	else if (tokencmpr(s, "exit0")) {
		t.set_type(Token::Exit0);
		t.set_content("exit0");
		s+=5;
	}
	else if (tokencmpr(s, "minsky")) {
		t.set_type(Token::Minsky);
		t.set_content("minsky");
		s+=6;
	}
	else if (tokencmpr(s, "machine")) {
		t.set_type(Token::Machine);
		t.set_content("machine");
		s+=7;
	}
	else if ((*s>='A' && *s<='Z') || (*s>='a' && *s<='z')) {
		t.set_content("");
		for (int i=0; ;i++) {
			if ((s[i]>='A' && s[i]<='Z') || (s[i]>='a' && s[i]<='z') || (s[i]>='0' && s[i]<='9')) {
				t.add_content(s[i]);
			}
			else if (tokencmpr(s+i, "")) {
				t.set_type(Token::Identifier);
				s+=i;
				break;
			}
			else {
				t.set_content("lexing error");
				return NULL;
			}
		}
	}
	else {
		char c;
		int i;
		i = 0;
		t.set_content("");
		c = s[i];
		while (!isdelim(c) && i<64) {
			t.add_content(c);
			i++;
			c = s[i];
		}
		return NULL;
	}
	if (next) {
		s = skip(s, " \t", 2);
	}
	return s;
}

void Lexer::nextToken(const char *s, Token &t) {
	_nextToken (s, t, false);
}

void Lexer::nextToken(const char *s, Token &t, const char** n) {
	*n = _nextToken (s, t, true);
}

void Lexer::setNext(const char *s) {
	pos = s;
}

void Lexer::getNext(Token &t) {
	nextToken(pos, t, &pos);
}

void Lexer::getNext(const char *s, Token &t) {
	pos = s;
	nextToken(pos, t, &pos);
}

void Lexer::peekNext(Token &t) {
	nextToken(pos, t);
}

