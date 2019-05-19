#pragma once
#include "Graph.h"
#include <string>
#include <vector>

class Coloring
{
public:
	Coloring();
	virtual ~Coloring();
	virtual void Run() = 0;
	virtual const Graph & ColoredGraph() const = 0;
	virtual const std::vector<size_t> & Colors() const = 0;
	virtual std::string Description() const = 0;
	virtual std::string Name() const = 0;
	virtual size_t NumberOfColors() const = 0;
	virtual double ColoringTime() const = 0; //In nanoseconds
};

