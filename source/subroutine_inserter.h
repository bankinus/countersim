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
	public:
		Subroutine_inserter(Context &c, std::vector<int> regs, std::vector<int> ex, std::vector<std::string> en) : parentcontext(c), registers(regs), exits(ex), exit_names(en){
			offset = parentcontext.current_line-1;
		}
		virtual void visit(Madd_command *command) override;
		virtual void visit(Msub_command *command) override;
};

