#include "Graph.h"

Graph::Graph(std::string name): _name(name)
{
}


Graph::~Graph()
{
}

size_t Graph::EdgesCount()
{
	return num_edges(*this);
}

size_t Graph::VerticesCount()
{
	return num_vertices(*this);
}

std::string Graph::GraphName()
{
	return _name;
}