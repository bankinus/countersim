#pragma once

typedef enum {Identifier, Number, Exit, Newline, EOP,
								Def, Main, Call, BracketL, BracketR, Colon,
							 	Madd, Msub,
								Uadd, Usub, Uclear, Ucopy, Ujmp,
								Preproc, Setreg, LRM, URM,
								nil}
					tokenType;

class Token
{
	private:
	char content[64]; 
	char* longcontent; 
	long long int numericalValue; 
	tokenType type;

	public:
	Token () : content(), longcontent((char*)0), type(nil), numericalValue(0) {
		content[0] = '\0';
		content[63] = '\0';
	}
	Token (const char *s, tokenType t) : longcontent((char*)0), type(t), numericalValue(0) {
		content[0] = '\0';
		content[63] = '\0';
	}

	tokenType get_type( );
	void set_type(tokenType t);

	const char* get_content();
	void set_content(const char *s);
	void set_content(const char c, unsigned long long i);

	long long int get_numerical_value();
	void set_numerical_value(long long int i);
};

