#pragma once
#include "simulator_command.h"

class Context {
	private:
		Simulator_command *subprogram[];
	public:
		long long int first_line;
		long long int last_line;
		long long int current_line;

		Context () {}

		long long int get_reg(std::string s);
		void set_reg(std::string s, long long int i);

		long long int get_line(std::string s);
		void set_line(std::string s, long long int i);
};

