#include "ZajacColoring.h"
#include <boost/timer.hpp>

std::string ZajacColoring::_name = "Zajac Coloring";
std::string ZajacColoring::_description = "Coloring with usage of Zajac algorithm: if graph is regular, use Zajac trick, otherwise do smalles last";

ZajacColoring::ZajacColoring(Graph & graph) : _graph(graph), _colors(graph.VerticesCount()), _time(-1.0)
{
}

ZajacColoring::~ZajacColoring()
{
}

void ZajacColoring::Run()
{
	boost::timer timer;
	timer.restart();

	_time = timer.elapsed();
}

const Graph & ZajacColoring::ColoredGraph()
{
	return _graph;
}

const std::vector<size_t>& ZajacColoring::Colors()
{
	return _colors;
}

std::string ZajacColoring::Description()
{
	return ZajacColoring::_description;
}

std::string ZajacColoring::Name()
{
	return ZajacColoring::_name;
}

double ZajacColoring::ColoringTime()
{
	return _time;
}

size_t ZajacColoring::NumberOfColors()
{
	return _number_of_colors;
}
