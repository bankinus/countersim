#pragma once

#ifdef DEBUG
#define lexertest() _lexertest()
#else
#define lexertest()
#endif

void _lexertest();

