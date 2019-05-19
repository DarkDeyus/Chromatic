#pragma once
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/qi_match.hpp>
#include <string>
#include <fstream>
#include "GraphException.h"
#include "Graph.h"

class DimacsParser
{
public:
	DimacsParser(const std::string & filename);
	~DimacsParser();

	Graph Read() 
	{
		Graph g(_filename);
		auto add_edge_ = [&g](size_t from, size_t to) { add_edge(from, to, g); };
		size_t vertices = 0, edges = 0;

		using namespace boost::spirit::qi;
		namespace px = boost::phoenix;
		uint_parser<size_t> num_;

		auto eoil = eol | eoi;
		auto comment = boost::proto::deep_copy(lexeme["c " >> *(char_ - eol) >> eoil] | eol);
		auto vertices_ = px::ref(vertices);
		auto edges_ = px::ref(edges);

		_file >> std::noskipws >> phrase_match(
			*comment >>
			("p" >> lit("edge") >> num_[vertices_ = _1] >> num_[edges_ = _1] >> eoil) >>
			repeat(edges_)[
				*comment >> ("e" >> num_ >> num_ >> eoil)[px::bind(add_edge_, _1 - 1, _2 - 1)]
			]
			>> *comment >> eoi
					, blank);

		if (!(_file.good() || _file.eof()))
		{
			throw GraphException("Failed to parse file " + _filename + " as Dimacs");
		}
		
		return g;
	}

private:
	std::ifstream _file;
	std::string _filename;
};

