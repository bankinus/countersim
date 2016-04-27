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
		long long unsigned int cyclecount;
		bool finished;
	public:
		Execution_visitor(Simulation &s) : simulation(s), next(1), cyclecount(0), finished(false) {}
		virtual ~Execution_visitor() {}

		size_t get_next() const;
		size_t get_cycle_count() const;

		virtual void visitc(Context &context) override;
		bool step_visitc(Context &context);
		bool get_finished() const;

		virtual void visit(Madd_command *command) override;
		virtual void visit(Msub_command *command) override;
		virtual void visit(Mmul_command *command) override;
		virtual void visit(Mdiv_command *command) override;
		virtual void visit(Uinc_command *command) override;
		virtual void visit(Udec_command *command) override;
		virtual void visit(Ucopy_command *command) override;
		virtual void visit(Uclear_command *command) override;
		virtual void visit(Ujump_command *command) override;
};

