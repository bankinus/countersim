#pragma once

class simulator_command
{
	public:
		virtual bool execute () = 0;
		virtual const char* toString() = 0;
};

class Madd : public simulator_command
{
};

class Msub : public simulator_command
{
};

class Parsing_error : public simulator_command
{
};

