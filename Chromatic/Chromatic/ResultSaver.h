#pragma once
#include <string>
#include <iostream>
#include "Graph.h"
#include "Coloring.h"

class ResultSaver
{
public:
	ResultSaver(const Coloring & coloring);
	~ResultSaver();
	void WriteResult(std::ostream & stream = std::cout);
private:
	Graph _graph;
	std::string _coloring_name;
	std::vector<size_t> _colors;
	std::string _coloring_desc;
	double _coloring_time;
};