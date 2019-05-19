#pragma once
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>

class Graph : public boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>
{
public:
	Graph(std::string name);
	~Graph();
	size_t EdgesCount();
	size_t VerticesCount();
	std::string GraphName();

private:
	std::string _name;
};
