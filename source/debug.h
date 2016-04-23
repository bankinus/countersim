#pragma once

#include <iostream>
#include "context.h"
#ifdef DEBUG
#else
#endif


#ifdef DEBUG
#define lexer_test() _lexer_test()
#define parse_Minsky_program_test() _parse_Minsky_program_test()
#define print_prog(con) _print_prog(con)
#else
#define lexer_test()
#define parse_Minsky_program_test()
#define print_prog(con)
#endif

#ifdef DEBUG
void _lexer_test();

void _parse_Minsky_program_test();

void _print_prog(Context *con);
#endif

#ifndef DEBUG
class DebugStream {
	public:
	DebugStream() {}

	template <typename T> DebugStream& operator<<(T t) {
		(void) t;
		return *this;
	}
	DebugStream& operator<<(std::ostream& (*f)(std::ostream&));
};

extern DebugStream debug;
#else
#define debug std::cout
#endif
