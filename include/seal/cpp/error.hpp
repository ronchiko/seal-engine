#pragma once

#ifdef __cplusplus

#include <stdexcept>

namespace seal {

class error : public std::exception {
public:
	error(const char* message);
};

}

#endif