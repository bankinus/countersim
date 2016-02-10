#pragma once
#include "simulator_command.h"

bool parse_simulator_program(char *s);

bool parse_config_command(char *s);

bool parse_Minsky_program(char *s);

bool parse_URM_program(char *s);

bool parse_URM_routine(char *s);

bool parse_Minsky_command(char *s, simulator_command **res);

bool parse_URM_command(char *s, simulator_command **res);

