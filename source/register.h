#pragma once

class Register {
	protected:
		long long unsigned int value;
	
	public:
		Register() : value(0) {}
		long long unsigned int get_value() const;
		void set_value(long long unsigned int val);
};

