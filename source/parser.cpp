#include "parser.h"
#include "token.h"
#include "lexer.h"
#include "simulator_command.h"
#include <string>

class Subroutine {
	private:
		Simulator_command *subprogram[];
	public:
		long long int first_line;
		long long int last_line;
		long long int current_line;

		Subroutine () {}

		long long int get_reg(std::string s) {
			//TODO
			return -1;
		}

		long long int get_command(std::string s) {
			//TODO
			return -1;
		}
};

bool parse_simulator_program(const char *s) {
	return false;
}

bool parse_config_command(const char *s) {
	return false;
}
bool parse_Minsky_program(const char *s) {
	return false;
}

bool parse_URM_program(const char *s) {
	return false;
}

bool parse_URM_routine(const char *s) {
	return false;
}

bool parse_Minsky_command(const char *s, Simulator_command **res, Subroutine &sub) {
	return false;
}

bool parse_URM_command(const char *s, Simulator_command** res, Subroutine &sub) {
	return false;
}

