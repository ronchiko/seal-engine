#include <seal/cpp/error.hpp>

seal::error::error(const char *message)
	: std::exception(message)
{}