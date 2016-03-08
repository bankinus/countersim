#pragma once

#include "routine_visitor.h"
class Execution_visitor;
class Simulation;
#include "simulation.h"
#include <stddef.h>

class Execution_visitor : public Routine_visitor {
	protected:
		Simulation &simulation;
		size_t next;
	public:
		Execution_visitor(Simulation &s) : simulation(s), next(1) {}

		virtual void visitc(Context &context) override;
		bool step_visitc(Context &context);

		virtual void visit(Madd_command *command) override;
		virtual void visit(Msub_command *command) override;
};

