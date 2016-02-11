#pragma once

#ifdef DEBUG
#define lexer_test() _lexer_test()
#define parse_Minsky_test() _parse_Minsky_test()
#else
#define lexer_test()
#define parse_Minsky_test()
#endif

#ifdef DEBUG
void _lexer_test();

void _parse_Minsky_test();
#endif

