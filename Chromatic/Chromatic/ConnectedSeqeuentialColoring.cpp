#include "ConnectedSeqeuentialColoring.h"
#include <boost/timer.hpp>

std::string ConnectedSeqeuentialColoring::_name = "Connected Sequential";
std::string ConnectedSeqeuentialColoring::_description = "Colors graph with BFS alghoritm";

ConnectedSeqeuentialColoring::ConnectedSeqeuentialColoring(Graph & graph) : _graph(graph), _colors(graph.VerticesCount()), _time(-1)
{
}


ConnectedSeqeuentialColoring::~ConnectedSeqeuentialColoring()
{
}

void ConnectedSeqeuentialColoring::Run()
{
	boost::timer timer;
	timer.restart();

	_time = timer.elapsed();
}

const Graph & ConnectedSeqeuentialColoring::ColoredGraph()
{
	return _graph;
}

const std::vector<size_t> & ConnectedSeqeuentialColoring::Colors()
{
	return _colors;
}

std::string ConnectedSeqeuentialColoring::Description()
{
	return ConnectedSeqeuentialColoring::_description;
}

std::string ConnectedSeqeuentialColoring::Name()
{
	return ConnectedSeqeuentialColoring::_name;
}

double ConnectedSeqeuentialColoring::ColoringTime()
{
	return _time;
}

size_t ConnectedSeqeuentialColoring::NumberOfColors()
{
	return _number_of_colors;
}
