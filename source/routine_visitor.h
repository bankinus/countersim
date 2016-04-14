#pragma once
class Context;
#include "simulator_command.h"
#include "context.h"
class Routine_visitor {
	public:
		Routine_visitor(){}
		~Routine_visitor(){}
		virtual void visitc(Context &context);
		virtual void visit(Madd_command *command);
		virtual void visit(Msub_command *command);
		virtual void visit(Mmul_command *command);
		virtual void visit(Mdiv_command *command);
		virtual void visit(Uinc_command *command);
		virtual void visit(Udec_command *command);
		virtual void visit(Ucopy_command *command);
		virtual void visit(Uclear_command *command);
		virtual void visit(Ujump_command *command);
		virtual void visit(Simulator_command *command);
};

