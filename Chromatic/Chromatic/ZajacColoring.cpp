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

struct Comparer
{
	static std::vector<size_t> degrees;
	bool operator()(const size_t& vd1, const size_t& vd2) const
	{
		return degrees[vd1] < degrees[vd2];
	}
};

std::vector<size_t> Comparer::degrees = std::vector<size_t>();

class ZajacColoringHelper
{
public:
	ZajacColoringHelper(Graph& g, std::vector<size_t> & colors, size_t & number_of_colors) : _graph(g), _colors(colors), _number_of_colors(number_of_colors), _orig_graph(g)
	{
	}

	void Color()
	{
		Comparer::degrees.clear();
		_number_of_colors.get() = 0;
		int k = 0;
		Comparer cmp;
		std::vector<size_t> degrees_count;
		auto example = _degree_heap.push(0);
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
			Comparer::degrees.push_back(degree);
			handlers.push_back(_degree_heap.push(i));
		}
		while (_graph.EdgesCount() > 0)
		{
			size_t popped = _degree_heap.top();
			size_t popped_degree = Comparer::degrees[popped];
			bool is_regular = false;
			size_t regular_degree = SIZE_MAX;
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
			if (!is_regular)
			{
				_degree_heap.pop();
				_color_stack.push(ColorPair(popped));
				degrees_count[Comparer::degrees[popped]]--;
				for (auto neighbour : make_iterator_range(boost::adjacent_vertices(popped, _graph)))
				{
					degrees_count[Comparer::degrees[neighbour]]--;
					Comparer::degrees[neighbour]--;
					_degree_heap.update(handlers[neighbour]);
					degrees_count[Comparer::degrees[neighbour]]++;
				}
				boost::remove_vertex(popped, _graph);
				continue;
			}
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
			if (v0 == SIZE_MAX)
			{
				for (auto vertex : boost::make_iterator_range(boost::vertices(_graph)))
				{
					_color_stack.push(vertex);
				}
				break;
			}
			path.push_back(v0);
			path.push_back(popped);
			path.push_back(v2);
			std::unordered_map<size_t, size_t> path_dict;
			path_dict.insert({ v0, 0 });
			path_dict.insert({ popped, 1 });
			path_dict.insert({ v2, 2 });
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
				for (int i = vjindex; i > 2; --i)
				{
					_color_stack.push(ColorPair(path[i]));
				}
				for (int i = vjindex + 1; i < _graph.VerticesCount(); ++i)
				{
					_color_stack.push(ColorPair(path[i]));
				}
				_color_stack.push(ColorPair(path[2]));
				_color_stack.push(ColorPair(path[0]));
				break;
			}
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
				for (int i = vjindex; i > 2; --i)
				{
					_color_stack.push(ColorPair(path[i]));
				}
				for (int i = vjindex + 1; i < _graph.VerticesCount(); ++i)
				{
					_color_stack.push(ColorPair(path[i]));
				}
				_color_stack.push(ColorPair(path[2]));
				_color_stack.push(ColorPair(path[0]));

				for (size_t i = 0; i < path.size(); ++i)
				{
					degrees_count[Comparer::degrees[path[i]]]--;
					Comparer::degrees[path[i]] = 0;
					_degree_heap.update(handlers[path[i]]);
					boost::remove_vertex(path[i], _graph);
					_degree_heap.pop();
				}
				continue;
			}

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

			for (size_t i = path_dict[vs]; i < path.size(); ++i)
			{
				degrees_count[Comparer::degrees[path[i]]]--;
				for (auto neighbour : make_iterator_range(boost::adjacent_vertices(path[i], _graph)))
				{
					degrees_count[Comparer::degrees[neighbour]]--;
					Comparer::degrees[neighbour]--;
					degrees_count[Comparer::degrees[neighbour]]++;
					_degree_heap.update(handlers[neighbour]);
					_degree_heap.pop();
				}
				boost::clear_vertex(path[i], _graph);
			}
		}

		while (!_color_stack.empty())
		{
			ColorPair pair = _color_stack.top();
			_color_stack.pop();
			if (pair.otherVertex != SIZE_MAX)
			{
				_colors.get()[pair.vertex] = _colors.get()[pair.otherVertex];
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
						return;
					}
					taken_colors[n_color] = true;
				}
			}
		}
	}
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
	std::stack<ColorPair> _color_stack;
	std::map<size_t, size_t> _color_map;
	std::reference_wrapper<std::vector<size_t>> _colors;
	std::reference_wrapper<size_t> _number_of_colors;
	boost::heap::fibonacci_heap<size_t, boost::heap::compare<Comparer>> _degree_heap;
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

	Graph g_copy = _graph;
	std::stack<size_t> color_stack;
	ZajacColoringHelper helper(_graph, _colors, _number_of_colors);
	helper.Color();

	auto end = std::chrono::system_clock::now();
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
