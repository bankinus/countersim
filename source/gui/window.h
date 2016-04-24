#pragma once
#include <string>
#include <QCommandLineParser>

void writeToConsole(const std::string &s);
int graphical_execution (int argc, char **argv, QCommandLineParser &parser);

