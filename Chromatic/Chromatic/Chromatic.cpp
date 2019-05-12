#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/qi_match.hpp>

template <typename Graph>
bool read_coloring_problem(std::istream& dimacs, Graph& g) {
	auto add_edge_ = [&g](size_t from, size_t to) { add_edge(from, to, g); };
	size_t vertices = 0, edges = 0;

	using namespace boost::spirit::qi;
	namespace px = boost::phoenix;
	uint_parser<size_t> num_;

	auto eoil = eol | eoi;
	auto comment = boost::proto::deep_copy(lexeme["c " >> *(char_ - eol) >> eoil] | eol);
	auto vertices_ = px::ref(vertices);
	auto edges_ = px::ref(edges);

	dimacs >> std::noskipws >> phrase_match(
		*comment >>
		("p" >> lit("edge") >> num_[vertices_ = _1] >> num_[edges_ = _1] >> eoil) >>
		repeat(edges_)[
			*comment >> ("e" >> num_ >> num_ >> eoil)[px::bind(add_edge_, _1 - 1, _2 - 1)]
		]
		>> *comment >> eoi
				, blank);

	return dimacs.good() || dimacs.eof();
}

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <fstream>
#include <iostream>

using namespace boost::graph;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;

int main() {
	Graph g2;
	std::ifstream input("../graphs/anna.col");
	if (read_coloring_problem(input, g2))
	{
		std::cout << "Nodes: " << num_vertices(g2) << "\n";
		std::cout << "Edges: " << num_edges(g2) << "\n";
		boost::write_graphviz(std::cout, g2);
		std::cout << "Nodes: " << num_vertices(g2) << "\n";
		std::cout << "Edges: " << num_edges(g2) << "\n";
	}
	else {
		std::cout << "Parse error\n";
	}
}


/*
An input file contains all the information about a graph needed to define a coloring problem.

In this format, nodes are numbered from 1 up to n edges in the graph.

Files are assumed to be well - formed and internally consistent : node identifier values are valid, nodes are defined uniquely, exactly m edges are defined, and so forth.

Comments.Comment lines give human - readable information about the file and are ignored by programs.Comment lines can appear anywhere in the file.Each comment line begins with a lower - case character c.


c This is an example of a comment line.

Problem line.There is one problem line per input file.The problem line must appear before any node or arc descriptor lines.For network instances, the problem line has the following format.

p FORMAT NODES EDGES

The lower - case character p signifies that this is the problem line.The FORMAT field is for consistency with the previous Challenge, and should contain the word ``edge''. The NODES field contains an integer value specifying n, the number of nodes in the graph.The EDGES field contains an integer value specifying m, the number of edges in the graph.

Edge Descriptors.There is one edge descriptor line for each edge the graph, each with the following format.Each edge(v, w) appears exactly once in the input file and is not repeated as(w, v).

e W V

The lower - case character e signifies that this is an edge descriptor line.For an edge(w, v) the fields W and V specify its endpoints.
*/