#pragma once
#include "routine_visitor.h"

class Routine_name_resolver : public Routine_visitor {
	protected:
		Context &context;
		void resolve_target(Target_command *command);
		void resolve_jump(Jump_command *command);
		void resolve_branch(Branch_command *command);
	public:
		Routine_name_resolver(Context &c) : context(c) {}
		virtual void visit(Madd_command *command) override;
		virtual void visit(Msub_command *command) override;
		virtual void visit(Mmul_command *command) override;
		virtual void visit(Mdiv_command *command) override;
};

