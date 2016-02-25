#pragma once
#include "simulator_command.h"
#include "context.h"
#include <unordered_map>

class Parser {
	std::unordered_map<std::string, Context> context_map;
	public:
	Parser () : context_map(){}
	Context *parse_simulator_program(const char *s);

	protected:
	bool parse_config_command(const char *s, Context *context, const char**next);

	Context *parse_Minsky_program(const char *s);

	Context *parse_URM_program(const char *s);

	Context *parse_URM_routine(const char *s);

	Context *parse_Minsky_routine(const char *s, Context *context);

	Context *parse_Minsky_sub_routine(const char *s);

	Context *parse_Minsky_main_routine(const char *s);

	bool parse_Minsky_command(const char *s, const char **next, Simulator_command **res, Context &con);

	bool parse_URM_command(const char *s, const char **next, Simulator_command **res, Context &con);
};

