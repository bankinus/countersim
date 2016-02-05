#pragma once

class simulator_command
{
	virtual bool execute ();
	virtual String toString();
	virtual simulatorCommand = 0;
}

class Madd : public simulator_command
{
}

class Msub : public simulator_command
{
}

class Parsing_error : public simulator_command
{
}

