#pragma once
class Simulator_command;
class Context;
class Madd_command;
class Msub_command;
class Routine_visitor {
	public:
		Routine_visitor();
		virtual void visit(Context context);
		virtual void visit(Madd_command *command);
		virtual void visit(Msub_command *command);
		virtual void visit(Simulator_command *command);
};
#include "simulator_command.h"
#include "context.h"

