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

const char * _nextToken(const char *s, class Token &t, bool next) {
	skip(s, " \t", 2);
	if (*s==0) {
		t.set_type(EOP);
		t.set_content("EOP");
	}
	else if (*s=='#') {
		t.set_type(Preproc);
		t.set_content("#");
	}
	else if (*s>='0' && *s<='9') {
		t.set_type(Number);
		//TODO insert actual number
		t.set_content("number");
	}
	else if (*s=='p' && (s[1]=='\t' || s[1]==' ')) {
		t.set_type(Uadd);
		t.set_content("p");
	}
	else if (*s=='d' && (s[1]=='\t' || s[1]==' ')) {
		t.set_type(Usub);
		t.set_content("d");
	}
	else if (*s=='c' && (s[1]=='\t' || s[1]==' ')) {
		t.set_type(Ucopy);
		t.set_content("c");
	}
	else if (*s=='o' && (s[1]=='\t' || s[1]==' ')) {
		t.set_type(Uclear);
		t.set_content("o");
	}
	else if (*s=='j' && (s[1]=='\t' || s[1]==' ')) {
		t.set_type(Ujmp);
		t.set_content("j");
	}
	else if (*s=='a' && s[1]=='d' && s[2]=='d' && (s[3]=='\t' || s[3]==' ')) {
		t.set_type(Madd);
		t.set_content("add");
	}
	else if (*s=='s' && s[1]=='u' && s[2]=='b' && (s[3]=='\t' || s[3]==' ')) {
		t.set_type(Msub);
		t.set_content("sub");
	}
	else if (*s=='d' && s[1]=='e' && s[2]=='f' && (s[3]=='\t' || s[3]==' ')) {
		t.set_type(Def);
		t.set_content("def");
	}
	else if (*s=='s' && s[1]=='e' && s[2]=='t' && (s[3]=='\t' || s[3]==' ')) {
		t.set_type(Setreg);
		t.set_content("set");
	}
	else if (*s=='l' && s[1]=='r' && s[2]=='m' && (s[3]=='\t' || s[3]==' ')) {
		t.set_type(LRM);
		t.set_content("lrm");
	}
	else if (*s=='u' && s[1]=='r' && s[2]=='m' && (s[3]=='\t' || s[3]==' ')) {
		t.set_type(URM);
		t.set_content("urm");
	}
	else if (*s=='c' && s[1]=='a' && s[2]=='l' && s[3]=='l' && (s[4]=='\t' || s[4]==' ')) {
		t.set_type(Call);
		t.set_content("call");
	}
	else if (*s=='m' && s[1]=='a' && s[2]=='i' && s[3]=='n' && (s[4]=='\t' || s[4]==' ')) {
		t.set_type(Main);
		t.set_content("main");
	}
	else if (*s>='a' && *s<='Z') {
		t.set_type(Identifier);
		//TODO insert actual identifier
		t.set_content("identifier");
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

void nextToken(const char *s, Token &t) {
	_nextToken (s, t, false);
}

void nextToken(const char *s, Token &t, const char** n) {
	*n = _nextToken (s, t, true);
}

