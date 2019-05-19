#include "ResultSaver.h"
#include <iostream>
#include <fstream>


ResultSaver::ResultSaver(Graph & g, std::string coloring_name, std::vector<size_t> colors) : _graph(g), _coloring_name(coloring_name), _colors(colors)
{
}


ResultSaver::~ResultSaver()
{
}

void ResultSaver::WriteResult()
{
	auto splitter_location = _graph.GraphName().find_last_of('/');
	if (splitter_location == std::string::npos)
		splitter_location = 0;
	else
		splitter_location++;

	std::string filename = _graph.GraphName().substr(splitter_location, std::string::npos);
	std::string result_filename = "../results/" + filename + "_" + _coloring_name + ".colored";
	std::ofstream result;
	result.open(result_filename);
	result << filename << std::endl;
	for (int i = 0; i < _graph.VerticesCount(); ++i)
	{
		result << i << " " << _colors[i] << std::endl;
	}
	result.close();
	return;
}
