#include "simulator_command.h"
#include <sstream>

long long int Simulator_command::get_line () {
	return line;
}

void Simulator_command::set_line (long long int i) {
	line = i;
}

long long int Simulator_command::get_actual_line () {
	return actual_line;
}

void Simulator_command::set_actual_line (long long int i) {
	actual_line = i;
}

void Simulator_command::accept (Routine_visitor *visitor) {
	visitor->visit(this);
}

void Target_command::set_target(long long int number) {
	target = number;
}

long long int Target_command::get_target() {
	return target;
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

long long int Jump_command::get_jump() {
	return jump;
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

long long int Branch_command::get_branch() {
	return branch;
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

