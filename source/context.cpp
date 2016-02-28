#include "context.h"

long long int Context::get_reg(std::string s) {
	return reg_map[s];
}

void Context::set_reg(std::string s, long long int i) {
	reg_map[s]=i;
}

long long int Context::get_line(std::string s) {
	return line_map[s];
}

void Context::set_line(std::string s, long long int i) {
	line_map[s]=i;
}

std::string Context::get_name(){
	return name;
}
void Context::set_name(std::string n){
	name = n;
}

void Context::add_command(Simulator_command* command) {
	subprogram.push_back(command);
}

std::vector<Simulator_command*> Context::get_program() {
	return subprogram;
}

