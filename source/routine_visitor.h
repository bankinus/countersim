#pragma once
class Simulator_command;
class Context;
class Madd_command;
class Msub_command;
class Mmul_command;
class Mdiv_command;
class Routine_visitor {
	public:
		Routine_visitor(){}
		~Routine_visitor(){}
		virtual void visitc(Context &context);
		virtual void visit(Madd_command *command);
		virtual void visit(Msub_command *command);
		virtual void visit(Mmul_command *command);
		virtual void visit(Mdiv_command *command);
		virtual void visit(Simulator_command *command);
};
#include "simulator_command.h"
#include "context.h"

