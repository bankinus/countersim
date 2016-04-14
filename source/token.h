#pragma once
#include <string>

class Token
{
	public:
	typedef enum tokenType_enum {Identifier, Number, Exit, Newline, EOP, Exit0,
	                     Def, Main, Call, BracketL, BracketR, Colon,
	                     Madd, Msub,
								Mmul, Mdiv,
	                     Uadd, Usub, Uclear, Ucopy, Ujmp,
	                     Preproc, Machine, Setreg,
								LRM, URM, Minsky, SRM,
	                     nil} tokenType;

	private:
	std::string content; 
	long long int numerical_value; 
	tokenType type;

	public:
	Token () : content(""), type(nil) {
	}
	Token (std::string s, tokenType t) : content(s), type(t) {
	}

	tokenType get_type( );
	void set_type(tokenType t);

	std::string get_content();
	void set_content(std::string);
	void add_content(const char c);

	long long int get_numerical_value();
	void set_numerical_value(long long int i);
	static std::string typeName(tokenType t);
};

typedef Token::tokenType tokenType;
