#include "ResultSaver.h"
#include <iostream>
#include <fstream>


ResultSaver::ResultSaver(const Coloring& coloring) : _graph(coloring.ColoredGraph()), _coloring_name(coloring.Name()), _colors(coloring.Colors()), _coloring_desc(coloring.Description()), _coloring_time(coloring.ColoringTime())
{
}


ResultSaver::~ResultSaver()
{
}

void ResultSaver::WriteResult(std::ostream & stream)
{
	std::ofstream result;
	stream << _graph.GraphName() << std::endl;
	stream << _coloring_name << std::endl;
	stream << _coloring_desc << std::endl;
	stream << _coloring_time << std::endl;
	for (int i = 0; i < _graph.VerticesCount(); ++i)
	{
		stream << i << " " << _colors[i] << std::endl;
	}
	stream << "" << std::endl;
	return;
}
