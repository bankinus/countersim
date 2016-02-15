#pragma once
class Routine_visitor;
#include <string>

class Simulator_command
{
	protected:
		long long int line;
	public:
		virtual ~Simulator_command(){}
		virtual void execute () = 0;
		virtual void accept (Routine_visitor *visitor);
		virtual std::string toString() = 0;

		long long int get_line();
		void set_line(long long int i);
};

class Jump_command : virtual public Simulator_command
{
	protected:
		long long int jump;
		std::string jump_name;
	public:
		Jump_command() : jump_name(""){}
		void set_jump(long long int branch);
		void set_jump(std::string s);
};

class Target_command : virtual public Simulator_command
{
	protected:
		/* target is denoted by integer value
		 * if target is positive register number equals target
		 * if target number < 0 target is the negative number of the argument
		 */
		long long int target;
		std::string target_name;
	public:
		Target_command() : target_name(""){}
		void set_target(long long int number);
		void set_target(std::string s);
};

class Branch_command : virtual public Simulator_command
{
	protected:
		long long int branch;
		std::string branch_name;
	public:
		Branch_command() : branch_name(""){}
		void set_branch(long long int branch);
		void set_branch(std::string s);
};

class Madd_command : public Target_command, public Jump_command
{
	public:
		virtual void execute ();
		virtual void accept (Routine_visitor *visitor);
		virtual std::string toString();
};

class Msub_command : public Target_command, public Jump_command, public Branch_command
{
	public:
		virtual void execute ();
		virtual void accept (Routine_visitor *visitor);
		virtual std::string toString();
};
#include "routine_visitor.h"

