#pragma once
#include <string>

class Simulator_command
{
	public:
		virtual void execute () = 0;
		virtual std::string toString() = 0;
};

class Jump_command : public Simulator_command
{
	protected:
		Simulator_command *next;
};

class Target_command : public Simulator_command
{
	protected:
		//TODO add target register
};

class Branch_command : public Simulator_command
{
	protected:
		Simulator_command *branch;
};

class Madd : public Target_command, Jump_command
{
	public:
		virtual void execute ();
		virtual std::string toString();
};

class Msub : public Target_command, Jump_command, Branch_command
{
	public:
		virtual void execute ();
		virtual std::string toString();
};

