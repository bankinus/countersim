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
	//TODO
}

