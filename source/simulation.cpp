#include "simulation.h"
#include "debug.h"
#include "execution_visitor.h"

void Simulation::set_machine(Machine m){
	machine = m;
}

Simulation::Machine Simulation::get_machine() {
	return machine;
}

Register *Simulation::get_register(size_t num) {
	if (registers.size()>num) {
		return &(registers[num]);
	}
	else if (max_reg<num || max_reg==0) {
		size_t old = registers.size();
		registers.resize(num+1);
		for (size_t i= old; i<num; i++) {
			registers[i] = Register();
		}
		return &(registers[num]);
	}
	else {
		return NULL;
	}
}

const std::vector<Register> Simulation::get_registers() {
	return registers;
}

void Simulation::run_routine(Context *context) {
	Execution_visitor(*this).visitc(*context);
}

size_t Simulation::get_max_reg() {
	return max_reg;
}

void Simulation::set_max_reg(size_t s){
	max_reg=s;
}

