#pragma once
#include <string>
#include "Graph.h"

class ResultSaver
{
public:
	ResultSaver(Graph & g, std::string coloring_name, std::vector<size_t> colors);
	~ResultSaver();
	void WriteResult();
private:
	Graph _graph;
	std::string _coloring_name;
	std::vector<size_t> _colors;
};