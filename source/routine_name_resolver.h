#pragma once
#include "routine_visitor.h"

class Routine_name_resolver : public Routine_visitor {
	protected:
		Context &context;
		void resolve_target(Target_command *command);
		void resolve_target2(Dual_target_command *command);
		void resolve_jump(Jump_command *command);
		void resolve_branch(Branch_command *command);
	public:
		Routine_name_resolver(Context &c) : context(c) {}
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

