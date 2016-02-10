#include "simulator_command.h"
#include <sstream>

void Madd::execute () {
}

std::string  Madd::toString () {
	std::stringstream ss;
	ss << "add " << "target" << " " << next;
	return ss.str();
}

void Msub::execute () {
}

std::string  Msub::toString () {
	std::stringstream ss;
	ss << "sub " << "target" << " " << next << " " << branch;
	return ss.str();
}
