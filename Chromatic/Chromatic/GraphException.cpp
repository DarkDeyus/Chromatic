#include "GraphException.h"

GraphException::GraphException(const std::string & message) : _message(message)
{
}

const char * GraphException::what() const noexcept
{
	return _message.c_str();
}

GraphException::~GraphException()
{
}
