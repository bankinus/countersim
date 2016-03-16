#pragma once
#include <boost/program_options.hpp>
#include <string>

void writeToConsole(std::string s);
int graphical_execution (int argc, char **argv, boost::program_options::variables_map v_map);

