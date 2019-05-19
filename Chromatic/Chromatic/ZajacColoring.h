#pragma once
#include "Coloring.h"
class ZajacColoring :
	public Coloring
{
public:
	ZajacColoring(Graph & graph);
	virtual ~ZajacColoring();

	virtual void Run() override;
	virtual const Graph & ColoredGraph() const override;
	virtual const std::vector<size_t>& Colors() const override;
	virtual std::string Description() const override;
	virtual std::string Name() const override;
	virtual double ColoringTime() const override;
	virtual size_t NumberOfColors() const override;
	size_t ZajacStepCounter() const;
private:
	Graph _graph;
	static std::string _name;
	static std::string _description;
	double _time;
	std::vector<size_t> _colors;
	size_t _number_of_colors;
	size_t _zajac_step_counter;
};

