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
	virtual const Graph & ColoredGraph() = 0;
	virtual const std::vector<size_t> & Colors() = 0;
	virtual std::string Description() = 0;
	virtual std::string Name() = 0;
	virtual size_t NumberOfColors() = 0;
	virtual double ColoringTime() = 0; //In nanoseconds
};

