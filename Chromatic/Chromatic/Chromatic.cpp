#include "DimacsParser.h"
#include "Graph.h"
#include "GraphException.h"
#include <iostream>
#include "ConnectedSeqeuentialColoring.h"
#include "ZajacColoring.h"
#include <boost/heap/fibonacci_heap.hpp>

int main() 
{
	std::string filename = "../graphs/DSJC1000-9.col";
	DimacsParser parser(filename);
	try
	{
		Graph g = parser.Read();
		ConnectedSeqeuentialColoring coloringCS(g);
		coloringCS.Run();
		std::cout << "Time: " << coloringCS.ColoringTime() << std::endl;
		std::cout << "Number of colors: " << coloringCS.NumberOfColors() << std::endl;
		for (int i = 0; i < g.VerticesCount(); ++i)
		{
			std::cout << "Color for " << i << " : " << coloringCS.Colors()[i] << std::endl;
		}

		ZajacColoring coloringZ(g);
		coloringZ.Run();
		std::cout << "Time: " << coloringZ.ColoringTime() << std::endl;
		std::cout << "Number of colors: " << coloringZ.NumberOfColors() << std::endl;
		std::cout << "Zajac's steps: " << coloringZ.ZajacStepCounter() << std::endl;
		for (int i = 0; i < g.VerticesCount(); ++i)
		{
			std::cout << "Color for " << i << " : " << coloringZ.Colors()[i] << std::endl;
		}
		return 0;
	}
	catch (GraphException e)
	{
		std::cout << "Exception thrown during parsing: " << e.what() << std::endl;
		return -1;
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