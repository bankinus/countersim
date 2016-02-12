#include "token.h"

tokenType Token::get_type()
{
	return type;
}

void Token::set_type(tokenType t)
{
	type = t;
}

std::string Token::get_content()
{
	return content;
}

void Token::set_content(std::string s)
{
	content=s;
}

void Token::add_content(const char c)
{
	content.append(1,c);
}

void Token::set_numerical_value(long long int i)
{
	numerical_value = i;
}

long long int Token::get_numerical_value()
{
	return numerical_value;
}

