#include "lexer.h"
#include <string>

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
		if (d[i] == *s) s++;
		else break;
	}
	return s;
}

bool tokencmpr (const char *s, const char *t) {
	for (; *t!=0; s++,t++){
		if (*s!=*t) return false;
	}
	if (*s==' ' || *s=='\t' || *s==0 || *s=='\n' || *s==':' || *s=='#'|| *s=='['|| *s==']') return true;
	else return false;
}

const char * _nextToken(const char *s, class Token &t, bool next) {
	s = skip(s, " \t", 2);
	if (s[0]==0) {
		t.set_type(EOP);
		t.set_content("EOP");
	}
	else if (s[0]=='\n') {
		t.set_type(Newline);
		t.set_content("\n");
		s+=1;
	}
	else if (s[0]=='#') {
		t.set_type(Preproc);
		t.set_content("#");
		s+=1;
	}
	else if (s[0]=='[') {
		t.set_type(BracketL);
		t.set_content("[");
		s+=1;
	}
	else if (s[0]==']') {
		t.set_type(BracketR);
		t.set_content("]");
		s+=1;
	}
	else if (s[0]==':') {
		t.set_type(Colon);
		t.set_content(":");
		s+=1;
	}
	else if (*s>='0' && *s<='9') {
		long long number = 0;
		for (int i=0; ;i++) {
			if (s[i]>='0' && s[i]<='9') {
				number*=10;
				number+=s[i]-'0';
				t.set_content(s[i], i);
			}
			else if (tokencmpr(s+i, "")) {
				t.set_type(Number);
				t.set_content('\0', i);
				s+=i;
				break;
			}
			else {
				t.set_content("lexing error");
				return NULL;
			}
		}
	}
	else if (tokencmpr(s, "p")) {
		t.set_type(Uadd);
		t.set_content("p");
		s+=1;
	}
	else if (tokencmpr(s, "d")) {
		t.set_type(Usub);
		t.set_content("d");
		s+=1;
	}
	else if (tokencmpr(s, "c")) {
		t.set_type(Ucopy);
		t.set_content("c");
		s+=1;
	}
	else if (tokencmpr(s, "o")) {
		t.set_type(Uclear);
		t.set_content("o");
		s+=1;
	}
	else if (tokencmpr(s, "j")) {
		t.set_type(Ujmp);
		t.set_content("j");
		s+=1;
	}
	else if (tokencmpr(s, "add")) {
		t.set_type(Madd);
		t.set_content("add");
		s+=3;
	}
	else if (tokencmpr(s, "sub")) {
		t.set_type(Msub);
		t.set_content("sub");
		s+=3;
	}
	else if (tokencmpr(s, "def")) {
		t.set_type(Def);
		t.set_content("def");
		s+=3;
	}
	else if (tokencmpr(s, "set")) {
		t.set_type(Setreg);
		t.set_content("set");
		s+=3;
	}
	else if (tokencmpr(s, "lrm")) {
		t.set_type(LRM);
		t.set_content("lrm");
		s+=3;
	}
	else if (tokencmpr(s, "urm")) {
		t.set_type(URM);
		t.set_content("urm");
		s+=3;
	}
	else if (tokencmpr(s, "call")) {
		t.set_type(Call);
		t.set_content("call");
		s+=4;
	}
	else if (tokencmpr(s, "main")) {
		t.set_type(Main);
		t.set_content("main");
		s+=4;
	}
	else if ((*s>='A' && *s<='Z') || (*s>='a' && *s<='z')) {
		for (int i=0; ;i++) {
			if ((s[i]>='A' && s[i]<='Z') || (s[i]>='a' && s[i]<='z') || (s[i]>='0' && s[i]<='9')) {
				t.set_content(s[i], i);
			}
			else if (tokencmpr(s+i, "")) {
				t.set_type(Identifier);
				t.set_content('\0', i);
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
		t.set_content("lexing error");
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

