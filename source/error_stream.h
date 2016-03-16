#include <iostream>
#include <sstream>

class Error_stream {
	private:
		std::stringstream buf;
		bool console;
	public:
		Error_stream() : console(true), buf(){}
		Error_stream(bool b) : console(b), buf(){}

		void set_console(bool b);
		void flush();
		Error_stream &operator<<(char val);
		Error_stream &operator<<(std::string val);
		Error_stream &operator<<(long long int val);
		Error_stream &operator<<(unsigned long long int val);
		Error_stream &operator<<(Error_stream &(*f)(Error_stream &));
		static Error_stream &endl(Error_stream &);
};

extern Error_stream error_stream;

