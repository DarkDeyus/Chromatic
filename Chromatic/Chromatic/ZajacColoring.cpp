#include "ZajacColoring.h"
#include <boost/timer.hpp>
#include <chrono>
#include <boost/heap/fibonacci_heap.hpp>
#include <map>

std::string ZajacColoring::_name = "Zajac Coloring";
std::string ZajacColoring::_description = "Coloring with usage of Zajac algorithm: if graph is regular, use Zajac trick, otherwise do smalles last";

ZajacColoring::ZajacColoring(Graph& graph) : _graph(graph), _colors(graph.VerticesCount(), SIZE_MAX), _time(-1.0), _zajac_step_counter(0)
{
}

ZajacColoring::~ZajacColoring()
{
}

class ZajacColoringHelper
{
public:
	ZajacColoringHelper(Graph& g, std::vector<size_t> & colors, size_t & number_of_colors) : _graph(g), _colors(colors), _number_of_colors(number_of_colors), _orig_graph(g)
	{
	}

	void Color()
	{
		//Init necessary variables and structures
		std::vector<size_t> degrees(_graph.VerticesCount());
		_number_of_colors.get() = 0;
		int k = 0;
		zajacSteps = 0;
		Comparer cmp;
		std::vector<size_t> degrees_count;
		auto example = _degree_heap.push(VertexDegree(0,0));
		_degree_heap.pop();
		std::vector<decltype(example)> handlers;
		for (size_t i = 0; i < _graph.VerticesCount(); ++i)
		{
			size_t degree = boost::degree(i, _graph);
			if (degree > k)
			{
				k = degree;
				while (degrees_count.size() <= k)
				{
					degrees_count.push_back(0);
				}
			}
			degrees_count[degree]++;
			degrees[i] = degree;
			handlers.push_back(_degree_heap.push(VertexDegree(i,degree)));
		}

		//Proccess all vertices
		while (!_degree_heap.empty())
		{
			size_t popped = _degree_heap.top().vertex;
			size_t popped_degree = degrees[popped];
			bool is_regular = false;
			size_t regular_degree = SIZE_MAX;
			//Check regularity
			for (size_t i = 0; i < degrees_count.size(); ++i)
			{
				if (degrees_count[i] > 0 && degrees_count[i] < _graph.VerticesCount())
					break;
				if (degrees_count[i] == 0)
					continue;
				is_regular = true;
				regular_degree = i;
				break;
			}

			//If is not regular, do Smallest Last
			if (!is_regular)
			{
				_degree_heap.pop();
				_color_stack.push(ColorPair(popped));
				degrees_count[degrees[popped]]--;
				for (auto neighbour : make_iterator_range(boost::adjacent_vertices(popped, _graph)))
				{
					degrees_count[degrees[neighbour]]--;
					degrees[neighbour]--;
					(*(handlers[neighbour])).degree = degrees[neighbour];
					_degree_heap.update(handlers[neighbour]);
					degrees_count[degrees[neighbour]]++;
				}
				boost::clear_vertex(popped, _graph);
				continue;
			}

			//Zajac's part - find first vertices on path
			zajacSteps++;
			std::vector<size_t> path;
			size_t v0 = SIZE_MAX, v2 = SIZE_MAX;
			for (auto neighbour1 : make_iterator_range(boost::adjacent_vertices(popped, _graph)))
			{
				for (auto neighbour3 : make_iterator_range(boost::adjacent_vertices(popped, _graph)))
				{
					if (neighbour3 == neighbour1)
						continue;
					bool is_adjacent = false;
					for (auto neighbour13 : make_iterator_range(boost::adjacent_vertices(neighbour1, _graph)))
					{
						if (neighbour13 == neighbour3)
						{
							is_adjacent = true;
							break;
						}
					}
					if (!is_adjacent)
					{
						v0 = neighbour1;
						v2 = neighbour3;
						break;
					}
				}
				if (v0 != SIZE_MAX)
				{
					break;
				}
			}

			//If all neighbours are connected, graph is not Delta(G) colorable - color in greedy way
			if (v0 == SIZE_MAX)
			{
				for (auto vertex : boost::make_iterator_range(boost::vertices(_graph)))
				{
					_color_stack.push(vertex);
				}
				break;
			}
			
			//Construct path
			path.push_back(v0);
			path.push_back(popped);
			path.push_back(v2);
			std::unordered_map<size_t, size_t> path_dict;
			path_dict.insert({ v0, 0 });
			path_dict.insert({ popped, 1 });
			path_dict.insert({ v2, 2 });

			//Try to find next vertex as long as its possible
			while (true)
			{
				if (path.size() == _graph.VerticesCount())
					break;
				size_t next = SIZE_MAX;
				for (auto neighbour : make_iterator_range(boost::adjacent_vertices(path[path.size() - 1], _graph)))
				{
					bool is_on_path = false;
					for (auto path_vertex : path)
					{
						if (path_vertex == neighbour)
						{
							is_on_path = true;
							break;
						}
					}
					if (!is_on_path)
					{
						next = neighbour;
						break;
					}
				}
				if (next == SIZE_MAX)
				{
					break;
				}
				path_dict.insert({ next, path.size() });
				path.push_back(next);
			}

			//If all vertices are on path
			if (path.size() == _graph.VerticesCount())
			{
				size_t vj;
				for (auto neighbour : make_iterator_range(boost::adjacent_vertices(path[1], _graph)))
				{
					if (neighbour != path[0] && neighbour != path[2])
					{
						vj = neighbour;
						break;
					}
				}
				size_t vjindex = path_dict[vj];
				_color_stack.push(ColorPair(path[1]));
				for (size_t i = vjindex; i > 2; --i)
				{
					_color_stack.push(ColorPair(path[i]));
				}
				for (size_t i = vjindex + 1; i < _graph.VerticesCount(); ++i)
				{
					_color_stack.push(ColorPair(path[i]));
				}
				_color_stack.push(ColorPair(path[2]));
				_color_stack.push(ColorPair(path[0]));
				break;
			}
			
			//If not all vertices are on path
			//Find cycle on path
			size_t vs = SIZE_MAX;
			for (size_t i = 0; i < path.size(); ++i)
			{
				for (auto neighbour : make_iterator_range(boost::adjacent_vertices(path[path.size() - 1], _graph)))
				{
					if (neighbour == path[i])
					{
						vs = path[i];
						break;
					}
				}
				if (vs != SIZE_MAX)
				{
					break;
				}
			}
			//Find vertex in cycle and its neighbour
			size_t neighbour_outside = SIZE_MAX;
			size_t neighbour_inside = SIZE_MAX;
			for (size_t i = path.size() - 2; i >= path_dict[vs]; --i)
			{
				for (auto neighbour : make_iterator_range(boost::adjacent_vertices(path[i], _graph)))
				{
					neighbour_outside = neighbour;
					for (size_t j = path_dict[vs]; j < path.size(); ++j)
					{
						if (neighbour == path[j])
						{
							neighbour_outside = SIZE_MAX;
							break;
						}
					}
					if (neighbour_outside != SIZE_MAX)
					{
						break;
					}
				}
				if (neighbour_outside != SIZE_MAX)
				{
					neighbour_inside = path[i];
					break;
				}
			}

			//If cycle (path) is disconnected from the rest of graph, do the same thing for coloring path as it would be whole graph, but don't stop main loop
			if (neighbour_outside == SIZE_MAX)
			{
				size_t vj;
				for (auto neighbour : make_iterator_range(boost::adjacent_vertices(path[1], _graph)))
				{
					if (neighbour != path[0] && neighbour != path[2])
					{
						vj = neighbour;
						break;
					}
				}
				size_t vjindex = path_dict[vj];
				_color_stack.push(ColorPair(path[1]));
				for (size_t i = vjindex; i > 2; --i)
				{
					_color_stack.push(ColorPair(path[i]));
				}
				for (size_t i = vjindex + 1; i < _graph.VerticesCount(); ++i)
				{
					_color_stack.push(ColorPair(path[i]));
				}
				_color_stack.push(ColorPair(path[2]));
				_color_stack.push(ColorPair(path[0]));

				for (size_t i = 0; i < path.size(); ++i)
				{
					degrees_count[degrees[path[i]]]--;
					degrees[path[i]] = 0;
					(*(handlers[path[i]])).degree = SIZE_MAX;
					_degree_heap.update(handlers[path[i]]);
					boost::remove_vertex(path[i], _graph);
					_degree_heap.pop();
				}
				continue;
			}

			//Color cycles
			size_t neighbour_inside_index = path_dict[neighbour_inside];
			for (size_t i = neighbour_inside_index; i >= path_dict[vs] && i !=SIZE_MAX; --i)
			{
				_color_stack.push(ColorPair(path[i]));
			}

			for (size_t i = path.size() - 1; i >= neighbour_inside_index + 2; --i)
			{
				_color_stack.push(ColorPair(path[i]));
			}
			_color_stack.push(ColorPair(path[neighbour_inside_index + 1], neighbour_outside));

			//Remove cycle from graph
			for (size_t i = path_dict[vs]; i < path.size(); ++i)
			{
				degrees_count[degrees[path[i]]]--;
				*(handlers[path[i]]) = VertexDegree((*(handlers[path[i]])).vertex, SIZE_MAX);
				_degree_heap.update(handlers[path[i]]);
				VertexDegree vd = _degree_heap.top();
				_degree_heap.pop();

				for (auto neighbour : make_iterator_range(boost::adjacent_vertices(path[i], _graph)))
				{
					degrees_count[degrees[neighbour]]--;
					degrees[neighbour]--;
					degrees_count[degrees[neighbour]]++;
					(*(handlers[neighbour])).degree = degrees[neighbour];
					_degree_heap.update(handlers[neighbour]);
				}
				boost::clear_vertex(path[i], _graph);
			}
		}

		//Color vertices from stack in greedy way
		while (!_color_stack.empty())
		{
			ColorPair pair = _color_stack.top();
			_color_stack.pop();
			//If vertex has designated color as the other, color him in the same way
			if (pair.otherVertex != SIZE_MAX)
			{
				_colors.get()[pair.vertex] = _colors.get()[pair.otherVertex];
				continue;
			}
			if (_number_of_colors.get() == 0)
			{
				_number_of_colors.get() = 1;
				_colors.get()[pair.vertex] = 0;
				continue;
			}
			auto neighbours = boost::adjacent_vertices(pair.vertex, _orig_graph.get());
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
						_colors.get()[pair.vertex] = _number_of_colors.get();
						_number_of_colors.get() += 1;
						break;
					}
					taken_colors[n_color] = true;
				}
			}
			if (_colors.get()[pair.vertex] == SIZE_MAX)
			{
				for (size_t i = 0; i < _number_of_colors; ++i)
				{
					if (!taken_colors[i])
					{
						_colors.get()[pair.vertex] = i;
						break;
					}
				}
			}
		}
	}
	int zajacSteps;
private:
	Graph _graph;
	struct ColorPair
	{
		ColorPair(size_t vertex, size_t otherVertex = SIZE_MAX)
		{
			this->vertex = vertex;
			this->otherVertex = otherVertex;
		}
		size_t vertex;
		size_t otherVertex;
	};
	struct VertexDegree
	{
		VertexDegree(size_t vertex, size_t degree)
		{
			this->vertex = vertex;
			this->degree = degree;
		}
		size_t vertex;
		size_t degree;
	};
	struct Comparer
	{
		bool operator()(const VertexDegree& vd1, const VertexDegree& vd2) const
		{
			int d1 = vd1.degree == SIZE_MAX ? 0 : (vd1.degree + 1);
			int d2 = vd2.degree == SIZE_MAX ? 0 : (vd2.degree + 1);
			return d1 > d2;
		}
	};
	std::stack<ColorPair> _color_stack;
	std::map<size_t, size_t> _color_map;
	std::reference_wrapper<std::vector<size_t>> _colors;
	std::reference_wrapper<size_t> _number_of_colors;
	boost::heap::fibonacci_heap<VertexDegree, boost::heap::compare<Comparer>> _degree_heap;
	std::reference_wrapper<Graph> _orig_graph;
};

static void ZajacColorRec(Graph& g, std::vector<size_t> colors, size_t& number_of_colors)
{

}

static void ZajacColor(Graph& g, std::vector<size_t>& colors, size_t& number_of_colors)
{
	Graph graph = g;
	number_of_colors = 0;
	ZajacColorRec(graph, colors, number_of_colors);
}

void ZajacColoring::Run()
{
	auto start = std::chrono::system_clock::now();

	std::stack<size_t> color_stack;
	ZajacColoringHelper helper(_graph, _colors, _number_of_colors);
	helper.Color();

	auto end = std::chrono::system_clock::now();
	_zajac_step_counter = helper.zajacSteps;
	_time = std::chrono::duration<double>(end - start).count();
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

size_t ZajacColoring::ZajacStepCounter()
{
	return _zajac_step_counter;
}