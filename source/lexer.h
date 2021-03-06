#pragma once
#include "token.h"

class Lexer
{
	private:
		const char*pos;
	public:
		Lexer () : pos(NULL) {}
		Lexer (const char *pos) : pos(pos) {}
		/// reads the next token, writes result to Token t
		/**
		 * nextToken
		 *
		 * lexes the string passed in s until it has read the next token
		 * preceding whitespaces(other than newline) are ignored
		 *
		 * @param s points to the beginning of the next token
		 * @param t stores the result
		 */
		static void nextToken(const char *s, Token &t);
		
		/// reads the next token, writes result to Token t and points to next token in n
		/**
		 * nextToken
		 *
		 * lexes the string passed in s until it has read the next token
		 * preceding whitespaces(other than newline) are ignored
		 * then finds begining of next token or end of code
		 *
		 * @param s points to the beginning of the next token
		 * @param t stores the result
		 * @param n stores the next token
		 */
		static void nextToken(const char *s, Token &t, const char** n);

		void setNext(const char*s);

		void getNext(Token &t);
		void getNext(const char *s, Token &t);

		void peekNext(Token &t);
};

