#include "Graph.h"

Graph::Graph()
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
