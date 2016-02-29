#include "simulation.h"
#include "execution_visitor.h"

void Simulation::set_machine(Machine m){
	machine = m;
}

Simulation::Machine Simulation::get_machine() {
	return machine;
}

Register *Simulation::get_register(size_t num) {
	if (registers.size()>num) {
		return &registers[num];
	}
	else if (max_reg<num) {
		size_t old = registers.size();
		registers.resize(num);
		for (int i= old; i<num; i++) {
			registers[i] = Register();
		}
		return &registers[num];
	}
	else {
		return NULL;
	}
}

void Simulation::run_routine(Context *context) {
	Execution_visitor(*this).visitc(*context);
}

