#pragma once
#include "simulator_command.h"

bool parse_simulator_program(const char *s);

bool parse_config_command(const char *s);

bool parse_Minsky_program(const char *s);

bool parse_URM_program(const char *s);

bool parse_URM_routine(const char *s);

bool parse_Minsky_command(const char *s, Simulator_command **res);

bool parse_URM_command(const char *s, Simulator_command **res);

