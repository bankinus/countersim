#pragma once
#include "simulator_command.h"
#include <unordered_map>
#include <vector>

class Context {
	private:
		std::vector<Simulator_command*> subprogram;
		std::string name;
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

		std::string get_name();
		void set_name(std::string n);

		void add_command(Simulator_command* command);
		std::vector<Simulator_command*> get_program();
};

