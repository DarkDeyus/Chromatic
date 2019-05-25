#include "DimacsParser.h"
#include "Graph.h"
#include "GraphException.h"
#include "ResultSaver.h"
#include <iostream>
#include "ConnectedSeqeuentialColoring.h"
#include "ZajacColoring.h"
#include <boost/lexical_cast.hpp>
#include <fstream>

std::vector<std::string> ParseArgs(int argc, char** argv)
{
	std::vector<std::string> args(argc);
	for (int i = 0; i < argc; ++i)
	{
		args[i] = argv[i];
	}
	return args;
}

int main(int argc, char **argv)
{
	std::vector<std::string> args = ParseArgs(argc, argv);
	if (args.size() < 3 || args.size() > 5)
	{
		std::cout << "Usage: chromatic path_to_graph 0|1 [vis] [output_file]" << std::endl;
		std::cout << "In second argument use 0 to color with connected sequential" << std::endl;
		std::cout << "In second argument use 1 to color with Zajac's alogoritm" << std::endl;
		std::cout << "Set vis to true (it's false by default) to get visualisable result" << std::endl;
		return -1; 
	}
	std::string filename = args[1];
	bool isZajac = boost::lexical_cast<bool>(args[2]);
	bool vis = args.size() >= 4 && boost::lexical_cast<bool>(args[3]);
	std::ofstream filestream;
	if (args.size() == 5)
	{
		filestream.open(args[4]);
	}
	std::ostream& stream = (args.size() != 5? std::cout : filestream);
	DimacsParser parser(filename);
	try
	{
		Graph g = parser.Read();
		Coloring & C = *(isZajac ? static_cast<Coloring*>(&ZajacColoring(g)) : static_cast<Coloring*>(&ConnectedSeqeuentialColoring(g)));
		C.Run();
		/*if (!isZajac)
		{
			ConnectedSeqeuentialColoring coloringCS(g);
			coloringCS.Run();
			std::vector<size_t> colors = coloringCS.Colors();
			if (!vis)
			{
				stream << "Graph name: " << coloringCS.ColoredGraph().GraphName() << std::endl;
				stream << "Time: " << coloringCS.ColoringTime() << std::endl;
				stream << "Number of colors: " << coloringCS.NumberOfColors() << std::endl;
				for (int i = 0; i < g.VerticesCount(); ++i)
				{
					stream << "Color for " << i << " : " << coloringCS.Colors()[i] << std::endl;
				}
			}
			else
			{
				ResultSaver saver(coloringCS);
				saver.WriteResult(stream);
			}
		}
		else
		{
			ZajacColoring coloringZ(g);
			coloringZ.Run();
			if (!vis)
			{
				stream << "Graph name: " << coloringZ.ColoredGraph().GraphName() << std::endl;
				stream << "Time: " << coloringZ.ColoringTime() << std::endl;
				stream << "Number of colors: " << coloringZ.NumberOfColors() << std::endl;
				stream << "Zajac's steps: " << coloringZ.ZajacStepCounter() << std::endl;
				for (int i = 0; i < g.VerticesCount(); ++i)
				{
					stream << "Color for " << i << " : " << coloringZ.Colors()[i] << std::endl;
				}
			}
			else
			{
				ResultSaver saver(coloringZ);
				saver.WriteResult(stream);
			}
		}*/
		if (vis)
		{
			ResultSaver saver(C);
			saver.WriteResult(stream);
		}
		else
		{
			stream << "Graph name: " << C.ColoredGraph().GraphName() << std::endl;
			stream << "Time: " << C.ColoringTime() << std::endl;
			stream << "Number of colors: " << C.NumberOfColors() << std::endl;

			for (int i = 0; i < g.VerticesCount(); ++i)
			{
				stream << "Color for " << i << " : " << C.Colors()[i] << std::endl;
			}
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