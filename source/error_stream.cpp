#include "error_stream.h"

Error_stream error_stream(true);

void Error_stream::flush () {
	if (console) {
		std::cerr.flush();
	}
	else {
		buf.flush();
	}
}

Error_stream &Error_stream::operator<<(char val) {
	if (console) {
		std::cerr<<val;
	}
	else {
		buf<<val;
	}
	return *this;
}

Error_stream &Error_stream::operator<<(std::string val) {
	if (console) {
		std::cerr<<val;
	}
	else {
		buf<<val;
	}
	return *this;
}

Error_stream &Error_stream::operator<<(long long int val) {
	if (console) {
		std::cerr<<val;
	}
	else {
		buf<<val;
	}
	return *this;
}

Error_stream &Error_stream::operator<<(Error_stream &(*f)(Error_stream &)) {
	f(*this);
	return *this;
}

Error_stream &Error_stream::endl(Error_stream &e) {
	if (e.console) {
		std::cerr<<std::endl;
	}
	else {
		e.buf<<std::endl;
	}
	return e;
}
