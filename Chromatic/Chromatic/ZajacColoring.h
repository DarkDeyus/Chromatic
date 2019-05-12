#pragma once
#include "Coloring.h"
class ZajacColoring :
	public Coloring
{
public:
	ZajacColoring(Graph & graph);
	virtual ~ZajacColoring();

	virtual void Run() override;
	virtual const Graph & ColoredGraph() override;
	virtual const std::vector<size_t>& Colors() override;
	virtual std::string Description() override;
	virtual std::string Name() override;
	virtual double ColoringTime() override;
	virtual size_t NumberOfColors() override;
private:
	Graph _graph;
	static std::string _name;
	static std::string _description;
	double _time;
	std::vector<size_t> _colors;
	size_t _number_of_colors;
};

