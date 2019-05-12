#include <boost/graph/breadth_first_search.hpp>
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

struct connected_sequential_visitor : public boost::default_bfs_visitor
{
public:
	size_t _number_of_colors;

	connected_sequential_visitor(std::vector<size_t> colors) : _colors(colors)
	{
		_number_of_colors = 0;
	}
	
	void examine_vertex(const Graph::vertex_descriptor &s, const Graph &g)
	{
		if (_number_of_colors == 0)
		{
			_colors.get()[s] = 0;
			_number_of_colors = 1;
			return;
		}

		auto neighbours = boost::adjacent_vertices(s, g);
		std::vector<bool> taken_colors(false, _number_of_colors);
		size_t colors_left = _number_of_colors;
		for (auto neighbour : make_iterator_range(neighbours))
		{
			if (!taken_colors[neighbour])
			{
				colors_left--;
				if (colors_left == 0)
				{
					_colors.get()[s] = _number_of_colors;
					_number_of_colors += 1;
					return;
				}
				taken_colors[neighbour] = true;
			}
		}

		for (int i = 0; i < _number_of_colors; ++i)
		{
			if (taken_colors[i] == false)
			{
				_colors.get()[s] = i;
				return;
			}
		}
	}

	std::reference_wrapper<std::vector<size_t>> _colors;
};

void ConnectedSeqeuentialColoring::Run()
{
	boost::timer timer;
	timer.restart();
	Graph::vertex_descriptor start_vertex = boost::vertex(0, _graph);
	connected_sequential_visitor vis(_colors);
	boost::graph::breadth_first_search(_graph, start_vertex, boost::default_bfs_visitor());
	_number_of_colors = vis._number_of_colors;

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
