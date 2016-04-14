#pragma once
#include "routine_visitor.h"
#include <vector>

class Subroutine_inserter : public Routine_visitor {
	protected:
		long long int offset;
		std::vector<int> registers;
		std::vector<int> exits;
		std::vector<std::string> exit_names;
		Context &parentcontext;
		void prologue(Simulator_command *command, Simulator_command *newcommand);
		void epilogue(Simulator_command *command, Simulator_command *newcommand);
		void copy_target(Target_command *command, Target_command *newcommand);
		void copy_target2(Dual_target_command *command, Dual_target_command *newcommand);
		void copy_jump(Jump_command *command, Jump_command *newcommand);
		void copy_branch(Branch_command *command, Branch_command *newcommand);
	public:
		Subroutine_inserter(Context &c, std::vector<int> regs, std::vector<int> ex, std::vector<std::string> en) : parentcontext(c), registers(regs), exits(ex), exit_names(en){
			offset = parentcontext.current_line-1;
		}
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

