#pragma once

#include <iostream>
#ifdef DEBUG
#else
#endif


#ifdef DEBUG
#define lexer_test() _lexer_test()
#define parse_Minsky_program_test() _parse_Minsky_program_test()
#else
#define lexer_test()
#define parse_Minsky_program_test()
#endif

#ifdef DEBUG
void _lexer_test();

void _parse_Minsky_program_test();
#endif

#ifndef DEBUG
class DebugStream {
	public:
	DebugStream() {}

	template <typename T> DebugStream& operator<<(T t) {
		return *this;
	}
	DebugStream& operator<<(std::ostream& (*f)(std::ostream&));
};

extern DebugStream debug;
#else
#define debug std::cout
#endif
