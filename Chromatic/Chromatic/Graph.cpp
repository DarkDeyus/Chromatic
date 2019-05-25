#include "Graph.h"

Graph::Graph(std::string name): _name(name)
{
}


Graph::~Graph()
{
}

size_t Graph::EdgesCount() const
{
	return num_edges(*this);
}

size_t Graph::VerticesCount() const
{
	return num_vertices(*this);
}

std::string Graph::GraphName() const
{
	return _name;
}