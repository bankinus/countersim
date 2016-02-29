#include "register.h"

long long unsigned int Register::get_value(){
	return value;
}

void Register::set_value(long long unsigned int val) {
	value=val;
}

