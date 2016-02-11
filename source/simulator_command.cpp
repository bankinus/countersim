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

void Target_command::set_target(std::string s) {
	target_name = s;
}

void Jump_command::set_jump(long long int number) {
	jump = number;
}

void Jump_command::set_jump(std::string s) {
	jump_name = s;
}

void Branch_command::set_branch(long long int number) {
	branch = number;
}

void Branch_command::set_branch(std::string s) {
	branch_name = s;
}

void Madd_command::execute () {
}

std::string  Madd_command::toString () {
	std::stringstream ss;
	ss << "add " << "target" << " " << jump;
	return ss.str();
}

void Msub_command::execute () {
}

std::string  Msub_command::toString () {
	std::stringstream ss;
	ss << "sub " << "target" << " " << jump << " " << branch;
	return ss.str();
}

