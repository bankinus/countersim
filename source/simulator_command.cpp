#include "simulator_command.h"
#include <sstream>

long long int Simulator_command::get_line () {
	return line;
}

void Simulator_command::set_line (long long int i) {
	line = i;
}

void Simulator_command::accept (Routine_visitor *visitor) {
	visitor->visit(this);
}

void Target_command::set_target(long long int number) {
	target = number;
}

std::string Target_command::get_target_name() {
	return target_name;
}

void Target_command::set_target_name(std::string s) {
	target_name = s;
}

void Jump_command::set_jump(long long int number) {
	jump = number;
}

std::string Jump_command::get_jump_name() {
	return jump_name;
}

void Jump_command::set_jump_name(std::string s) {
	jump_name = s;
}

void Branch_command::set_branch(long long int number) {
	branch = number;
}

std::string Branch_command::get_branch_name() {
	return branch_name;
}


void Branch_command::set_branch_name(std::string s) {
	branch_name = s;
}

void Madd_command::execute () {
}

std::string  Madd_command::toString () {
	std::stringstream ss;
	ss << "add " << target << " " << jump;
	return ss.str();
}

void Madd_command::accept (Routine_visitor *visitor) {
	visitor->visit(this);
}

void Msub_command::execute () {
}

std::string  Msub_command::toString () {
	std::stringstream ss;
	ss << "sub " << target << " " << jump << " " << branch;
	return ss.str();
}

void Msub_command::accept (Routine_visitor *visitor) {
	visitor->visit(this);
}

