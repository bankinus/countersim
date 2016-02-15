#pragma once
#include "routine_visitor.h"

class Routine_name_resolver : public Routine_visitor {
	protected:
		Context &context;
	public:
		Routine_name_resolver(Context &c) : context(c) {}
		virtual void visit(Madd_command *command) override;
		virtual void visit(Msub_command *command) override;
};

