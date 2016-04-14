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

std::string Token::typeName(tokenType t){
	switch (t) {
		case Identifier:
			return "NAME";
		case Number:
			return "INTEGER";
		case Newline:
			return "NEWLINE";
		case EOP:
			return "EOP";
		case Exit0:
			return "exit0";
		case Def:
			return "def";
		case Main:
			return "main";
		case Call:
			return "call";
		case BracketL:
			return "[";
		case BracketR:
			return "]";
		case Colon:
			return ":";
		case Madd:
			return "add";
		case Msub:
			return "sub";
		case Mmul:
			return "mul";
		case Mdiv:
			return "div";
		case Uinc:
			return "inc";
		case Udec:
			return "dec";
		case Uclear:
			return "clr";
		case Ucopy:
			return "cpy";
		case Ujump:
			return "jmp";
		case Preproc:
			return "#";
		case Machine:
			return "machine";
		case Setreg:
			return "set";
		case LRM:
			return "lrm";
		case URM:
			return "urm";
		case Minsky:
			return "minsky";
		case SRM:
			return "srm";
		case nil:
			return "nil";
		default:
			return "_unnamed_type";
	}
}

