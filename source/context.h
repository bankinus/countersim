#pragma once
#include "simulator_command.h"
#include <unordered_map>

class Context {
	private:
		Simulator_command **subprogram;
		std::unordered_map<std::string, long long int> reg_map;
		std::unordered_map<std::string, long long int> line_map;
	public:
		long long int first_line;
		long long int last_line;
		long long int current_line;

		Context () : reg_map(), line_map(){}

		long long int get_reg(std::string s);
		void set_reg(std::string s, long long int i);

		long long int get_line(std::string s);
		void set_line(std::string s, long long int i);
};

