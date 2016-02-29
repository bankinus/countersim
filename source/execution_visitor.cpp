#include "execution_visitor.h"

void Execution_visitor::visitc(Context &context) {
	next = 1;
	while (next!=0) {
		Simulator_command *command = context.get_program()[next-1];
		command->accept(this);
	}
}

void Execution_visitor::visit(Madd_command *command) {
	Register *reg = simulation.get_register(command->get_target());
	reg->set_value(reg->get_value()+1);
	next = command->get_jump();
}

void Execution_visitor::visit(Msub_command *command) {
	Register *reg = simulation.get_register(command->get_target());
	if (reg->get_value()!=0) {
		reg->set_value(reg->get_value()-1);
		next = command->get_jump();
	}
	else {
		next = command->get_branch();
	}
}

