#pragma once
#include <string>

class Simulator_command
{
	protected:
		long long int line;
	public:
		virtual void execute () = 0;
		virtual std::string toString() = 0;

		long long int get_line();
		void set_line(long long int i);
};

class Jump_command : virtual public Simulator_command
{
	protected:
		long long int next;
	public:
		void set_jump(long long int branch);
};

class Target_command : virtual public Simulator_command
{
	protected:
		/* target is denoted by integer value
		 * if target is positive register number equals target
		 * if target number < 0 target is the negative number of the argument
		 */
		long long int target;
	public:
		void set_target(long long int number);
};

class Branch_command : virtual public Simulator_command
{
	protected:
		long long int branch;
	public:
		void set_branch(long long int branch);
};

class Madd_command : public Target_command, public Jump_command
{
	public:
		virtual void execute ();
		virtual std::string toString();
};

class Msub_command : public Target_command, public Jump_command, public Branch_command
{
	public:
		virtual void execute ();
		virtual std::string toString();
};

