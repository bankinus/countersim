#include "token.h"

tokenType Token::get_type()
{
	return type;
}

void Token::set_type(tokenType t)
{
	type = t;
}

const char * Token::get_content()
{
	if (longcontent) return longcontent;
	else return content;
}

void Token::set_content(const char *s)
{
	int i;
	for (i=0; s[i]!=0; i++) {
		if (i >= 63) {
			//TODO long content
			break;
		}
		content[i] = s[i];
	}
	content[i] = '\0';
}

void Token::set_content(const char c, unsigned long long i)
{
		if (i >= 63) {
			//TODO long content
			return;
		}
		content[i] = c;
}

