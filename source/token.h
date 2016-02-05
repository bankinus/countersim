#pragma once
#include <String>

public enum tokenType {Identifier, Number, Exit, Newline,
								Def, Main, Call,
							 	Madd, Msub,
								Uadd, Usub, Uclear, Ucopy, Ujmp,
								Preproc, Setreg, LRM, URM,
								nil};

class Token
{
	private String content; 
	private numericalValue; 
	tokenType type;
	Token () : content(""), type(nil), numericalValue(0);
	Token (String s, tokenType t) : content(s), type(t), numericalValue(0);
	String getContent();
	void setContent(String s);
}

