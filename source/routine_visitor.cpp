#include "routine_visitor.h"

void Routine_visitor::visitc(Context &context) {
	for (Simulator_command *command: context.get_program()) {
		command->accept(this);
	}
}
void Routine_visitor::visit(Madd_command *command) {
	return;
}
void Routine_visitor::visit(Msub_command *command) {
	return;
}
void Routine_visitor::visit(Mmul_command *command) {
	return;
}
void Routine_visitor::visit(Mdiv_command *command) {
	return;
}
void Routine_visitor::visit(Simulator_command *command) {
	return;
}
