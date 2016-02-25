#include "routine_name_resolver.h"

void Routine_name_resolver::visit(Madd_command *command) {
	if (command->get_target_name()!="") {
		command->set_target(context.get_reg(command->get_target_name()));
	}
	if (command->get_jump_name()!="") {
		command->set_jump(context.get_line(command->get_jump_name()));
	}
}

void Routine_name_resolver::visit(Msub_command *command) {
	if (command->get_target_name()!="") {
		command->set_target(context.get_reg(command->get_target_name()));
	}
	if (command->get_jump_name()!="") {
		command->set_jump(context.get_line(command->get_jump_name()));
	}
	if (command->get_branch_name()!="") {
		command->set_branch(context.get_line(command->get_branch_name()));
	}
}

