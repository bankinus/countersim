#pragma once
#include "token.h"
#include "simulator_command.h"

bool parse_simulator_program(char *s);

bool parse_Minsky_program(char *s);

bool parse_Minsky_routine(char *s);

simulator_command parse_Minsky_command(char *s);

simulator_command parse_URM_command(char *s);

