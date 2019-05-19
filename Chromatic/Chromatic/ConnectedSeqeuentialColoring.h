#pragma once
#include "Coloring.h"
class ConnectedSeqeuentialColoring :
	public Coloring
{
public:
	ConnectedSeqeuentialColoring(Graph & graph);
	virtual ~ConnectedSeqeuentialColoring();
	
	virtual void Run() override;
	virtual const Graph & ColoredGraph() const override;
	virtual const std::vector<size_t> & Colors() const override;
	virtual std::string Description() const override;
	virtual std::string Name() const override;
	virtual double ColoringTime() const override;
	virtual size_t NumberOfColors() const override;
private:
	double _time;
	static std::string _name;
	static std::string _description;
	std::vector<size_t> _colors;
	Graph _graph;
	size_t _number_of_colors;
};

