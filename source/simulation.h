#pragma once

#include "register.h"
#include "context.h"
#include <vector>
#include <stddef.h>


/*
 *	represents the state of a program in execution
 */
class Simulation {
	public:
		typedef enum {Minsky, Urm, Lrm} Machine;
	protected:
		size_t max_reg;
		std::vector<Register> registers;
		Machine machine;
	public:
		Simulation() : registers() {}
		void  set_machine(Machine machine);
		Machine get_machine();
		Register *get_register(size_t num);
		void run_routine(Context *context);
};

