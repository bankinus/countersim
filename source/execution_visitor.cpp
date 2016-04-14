#include "execution_visitor.h"

void Execution_visitor::visitc(Context &context) {
	while (next!=0) {
		step_visitc(context);
	}
}

bool Execution_visitor::step_visitc(Context &context) {
	if (next==0) return false;
	Simulator_command *command = context.get_program()[next-1];
	command->accept(this);
	cyclecount++;
	if (next==0) return false;
	return true;
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

void Execution_visitor::visit(Mmul_command *command) {
	Register *reg = simulation.get_register(0);
	reg->set_value(reg->get_value()*command->get_target());
	next = command->get_jump();
}

void Execution_visitor::visit(Mdiv_command *command) {
	Register *reg = simulation.get_register(0);
	if (reg->get_value()%command->get_target()==0) {
		reg->set_value(reg->get_value()/command->get_target());
		next = command->get_jump();
	}
	else {
		next = command->get_branch();
	}
}

void Execution_visitor::visit(Uinc_command *command) {
	Register *reg = simulation.get_register(command->get_target());
	reg->set_value(reg->get_value()+1);
	next++;
}

void Execution_visitor::visit(Udec_command *command) {
	Register *reg = simulation.get_register(command->get_target());
	if (reg->get_value()!=0) {
		reg->set_value(reg->get_value()-1);
	}
	next++;
}

void Execution_visitor::visit(Ucopy_command *command) {
	Register *reg = simulation.get_register(command->get_target());
	Register *reg2 = simulation.get_register(command->get_target2());
	reg2->set_value(reg->get_value()-1);
	next++;
}

void Execution_visitor::visit(Uclear_command *command) {
	Register *reg = simulation.get_register(command->get_target());
	reg->set_value(0);
	next++;
}

void Execution_visitor::visit(Ujump_command *command) {
	if (command->get_jump()!=0){
		next = command->get_jump();
	}
	else {
		Register *reg = simulation.get_register(command->get_target());
		if (reg->get_value()==0) {
			next = command->get_branch();
		}
		else {
			next++;
		}
	}
}

size_t Execution_visitor::get_next() const{
	return next;
}

size_t Execution_visitor::get_cycle_count() const{
	return cyclecount;
}


