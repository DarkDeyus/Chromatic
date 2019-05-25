#pragma once
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>

class Graph : public boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>
{
public:
	Graph(std::string name);
	~Graph();
	size_t EdgesCount() const;
	size_t VerticesCount() const;
	std::string GraphName() const;

private:
	std::string _name;
};
