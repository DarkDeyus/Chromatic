#include <boost/graph/breadth_first_search.hpp>
#include "ConnectedSeqeuentialColoring.h"
#include <boost/timer.hpp>
#include <chrono>

std::string ConnectedSeqeuentialColoring::_name = "Connected Sequential";
std::string ConnectedSeqeuentialColoring::_description = "Colors graph with BFS alghoritm";

ConnectedSeqeuentialColoring::ConnectedSeqeuentialColoring(Graph & graph) : _graph(graph), _colors(graph.VerticesCount(), SIZE_MAX), _time(-1)
{
}


ConnectedSeqeuentialColoring::~ConnectedSeqeuentialColoring()
{
}

struct connected_sequential_visitor : public boost::default_bfs_visitor
{
public:

	connected_sequential_visitor(std::vector<size_t>& colors, size_t& number_of_colors) : _colors(colors), _number_of_colors(number_of_colors)
	{
		_number_of_colors.get() = 0;
	}
	
	void examine_vertex(const Graph::vertex_descriptor &s, const Graph &g)
	{
		if (_number_of_colors.get() == 0)
		{
			_colors.get()[s] = 0;
			_number_of_colors.get() = 1;
			return;
		}

		auto neighbours = boost::adjacent_vertices(s, g);
		std::vector<bool> taken_colors(_number_of_colors.get(), false);
		size_t colors_left = _number_of_colors.get();
		for (auto neighbour : make_iterator_range(neighbours))
		{
			size_t n_color = _colors.get()[neighbour];
			if (n_color == SIZE_MAX)
				continue;
			if (!taken_colors[n_color])
			{
				colors_left--;
				if (colors_left == 0)
				{
					_colors.get()[s] = _number_of_colors.get();
					_number_of_colors.get() += 1;
					return;
				}
				taken_colors[n_color] = true;
			}
		}

		for (int i = 0; i < _number_of_colors.get(); ++i)
		{
			if (taken_colors[i] == false)
			{
				_colors.get()[s] = i;
				return;
			}
		}
	}

	std::reference_wrapper<std::vector<size_t>> _colors;
	std::reference_wrapper<size_t> _number_of_colors;
};

void ConnectedSeqeuentialColoring::Run()
{
	auto start = std::chrono::system_clock::now();
	connected_sequential_visitor vis(_colors, _number_of_colors);
	
	boost::breadth_first_search(_graph, boost::vertex(0, _graph), visitor(vis));
	_number_of_colors = vis._number_of_colors;

	auto end = std::chrono::system_clock::now();
	_time = std::chrono::duration<double>(end - start).count();
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
