#pragma once
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>

class Graph : public boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>
{
public:
	Graph();
	~Graph();
	size_t EdgesCount();
	size_t VerticesCount();
};

