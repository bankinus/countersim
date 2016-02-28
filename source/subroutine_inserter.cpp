#include "subroutine_inserter.h"
#include "debug.h"
void Subroutine_inserter::visit(Madd_command *command) {
	Madd_command *newcommand = new Madd_command();
	newcommand->set_line(command->get_line());
	if (command->get_target()>=0) {
		newcommand->set_target(command->get_target());
	}
	else {
		newcommand->set_target(registers[-(command->get_target())]);
	}
	if (command->get_jump()>=0) {
		newcommand->set_jump(command->get_jump()+offset);
	}
	else {
		if (exits[-(command->get_jump())]!=0) {
			newcommand->set_jump(exits[-(command->get_jump())]);
		}
		else {
			newcommand->set_jump_name(exit_names[-(command->get_jump())]);
		}
	}
	parentcontext.add_command(newcommand);
	parentcontext.current_line++;
}

void Subroutine_inserter::visit(Msub_command *command) {
	Msub_command *newcommand = new Msub_command();
	newcommand->set_line(command->get_line());
	if (command->get_target()>=0) {
		newcommand->set_target(command->get_target());
	}
	else {
		newcommand->set_target(registers[-(command->get_target())]);
	}
	if (command->get_jump()>=0) {
		newcommand->set_jump(command->get_jump()+offset);
	}
	else {
		if (exits[-(command->get_jump())]!=0) {
			newcommand->set_jump(exits[-(command->get_jump())]);
		}
		else {
			newcommand->set_jump_name(exit_names[-(command->get_jump())]);
		}
	}
	if (command->get_branch()>=0) {
		newcommand->set_branch(command->get_jump()+offset);
	}
	else {
		if (exits[-(command->get_branch())]!=0) {
			newcommand->set_branch(exits[-(command->get_branch())]);
		}
		else {
			newcommand->set_branch_name(exit_names[-(command->get_branch())]);
		}
	}
	parentcontext.add_command(newcommand);
	parentcontext.current_line++;
}

