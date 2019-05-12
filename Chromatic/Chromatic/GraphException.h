#pragma once
#include <exception>
#include <string>

class GraphException : public std::exception
{
public:
	GraphException(const std::string & message);
	const char *what() const noexcept override;
	virtual ~GraphException();
private:
	std::string _message;
};

