#include "subroutine_inserter.h"
#include "debug.h"

void Subroutine_inserter::prologue(Simulator_command *command, Simulator_command *newcommand){
	newcommand->set_line(command->get_line());
	newcommand->set_actual_line(command->get_actual_line());
	newcommand->set_breakpoint(command->get_breakpoint());
}

void Subroutine_inserter::epilogue(Simulator_command *command, Simulator_command *newcommand){
	(void) command;
	parentcontext.add_command(newcommand);
	parentcontext.current_line++;
}

void Subroutine_inserter::copy_target(Target_command *command, Target_command *newcommand){
	if (command->get_target()>=0) {
		newcommand->set_target(command->get_target());
	}
	else {
		newcommand->set_target(registers[-(command->get_target())]);
	}
}

void Subroutine_inserter::copy_target2(Dual_target_command *command, Dual_target_command *newcommand){
	if (command->get_target2()>=0) {
		newcommand->set_target2(command->get_target2());
	}
	else {
		newcommand->set_target2(registers[-(command->get_target2())]);
	}
}

void Subroutine_inserter::copy_jump(Jump_command *command, Jump_command *newcommand){
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
}

void Subroutine_inserter::copy_branch(Branch_command *command, Branch_command *newcommand){
	if (command->get_branch()>=0) {
		newcommand->set_branch(command->get_branch()+offset);
	}
	else {
		if (exits[-(command->get_branch())]!=0) {
			newcommand->set_branch(exits[-(command->get_branch())]);
		}
		else {
			newcommand->set_branch_name(exit_names[-(command->get_branch())]);
		}
	}
}

void Subroutine_inserter::visit(Madd_command *command) {
	Madd_command *newcommand = new Madd_command();

	prologue(command, newcommand);

	copy_target(command, newcommand);
	copy_jump(command, newcommand);

	epilogue(command, newcommand);
}

void Subroutine_inserter::visit(Msub_command *command) {
	Msub_command *newcommand = new Msub_command();

	prologue(command, newcommand);

	copy_target(command, newcommand);
	copy_jump(command, newcommand);
	copy_branch(command, newcommand);

	epilogue(command, newcommand);
}

void Subroutine_inserter::visit(Mmul_command *command) {
	Mmul_command *newcommand = new Mmul_command();

	prologue(command, newcommand);

	copy_target(command, newcommand);
	copy_jump(command, newcommand);

	epilogue(command, newcommand);
}

void Subroutine_inserter::visit(Mdiv_command *command) {
	Mdiv_command *newcommand = new Mdiv_command();

	prologue(command, newcommand);

	copy_target(command, newcommand);
	copy_jump(command, newcommand);
	copy_branch(command, newcommand);

	epilogue(command, newcommand);
}

void Subroutine_inserter::visit(Uinc_command *command) {
	Uinc_command *newcommand = new Uinc_command();

	prologue(command, newcommand);

	copy_target(command, newcommand);

	epilogue(command, newcommand);
}

void Subroutine_inserter::visit(Udec_command *command) {
	Udec_command *newcommand = new Udec_command();

	prologue(command, newcommand);

	copy_target(command, newcommand);

	epilogue(command, newcommand);
}

void Subroutine_inserter::visit(Ucopy_command *command) {
	Ucopy_command *newcommand = new Ucopy_command();

	prologue(command, newcommand);

	copy_target(command, newcommand);
	copy_target2(command, newcommand);

	epilogue(command, newcommand);
}

void Subroutine_inserter::visit(Uclear_command *command) {
	Uclear_command *newcommand = new Uclear_command();

	prologue(command, newcommand);

	copy_target(command, newcommand);

	epilogue(command, newcommand);
}

void Subroutine_inserter::visit(Ujump_command *command) {
	Ujump_command *newcommand = new Ujump_command();

	prologue(command, newcommand);

	copy_target(command, newcommand);
	copy_jump(command, newcommand);
	copy_branch(command, newcommand);

	epilogue(command, newcommand);
}

