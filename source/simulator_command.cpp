#include "simulator_command.h"
#include <sstream>

long long int Simulator_command::get_line () {
	return line;
}

void Simulator_command::set_line (long long int i) {
	line = i;
}

void Target_command::set_target(long long int number) {
	target = number;
}

void Jump_command::set_jump(long long int number) {
	next = number;
}

void Branch_command::set_branch(long long int number) {
	branch = number;
}

void Madd_command::execute () {
}

std::string  Madd_command::toString () {
	std::stringstream ss;
	ss << "add " << "target" << " " << next;
	return ss.str();
}

void Msub_command::execute () {
}

std::string  Msub_command::toString () {
	std::stringstream ss;
	ss << "sub " << "target" << " " << next << " " << branch;
	return ss.str();
}

