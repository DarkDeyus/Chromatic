// Chromatic.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

int main()
{
    std::cout << "Hello World!\n"; 
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

/*
An input file contains all the information about a graph needed to define a coloring problem.

In this format, nodes are numbered from 1 up to n edges in the graph.

Files are assumed to be well - formed and internally consistent : node identifier values are valid, nodes are defined uniquely, exactly m edges are defined, and so forth.

Comments.Comment lines give human - readable information about the file and are ignored by programs.Comment lines can appear anywhere in the file.Each comment line begins with a lower - case character c.


c This is an example of a comment line.

Problem line.There is one problem line per input file.The problem line must appear before any node or arc descriptor lines.For network instances, the problem line has the following format.

p FORMAT NODES EDGES

The lower - case character p signifies that this is the problem line.The FORMAT field is for consistency with the previous Challenge, and should contain the word ``edge''. The NODES field contains an integer value specifying n, the number of nodes in the graph.The EDGES field contains an integer value specifying m, the number of edges in the graph.

Edge Descriptors.There is one edge descriptor line for each edge the graph, each with the following format.Each edge(v, w) appears exactly once in the input file and is not repeated as(w, v).

e W V

The lower - case character e signifies that this is an edge descriptor line.For an edge(w, v) the fields W and V specify its endpoints.
*/