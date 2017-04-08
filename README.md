# lipton-tarjan
Implementation of the Lipton-Tarjan Planar Separator Theorem

You will need to install graphviz to visualize the graphs

A graph file is just a list of edges

lt [graphfilename]

	Calculates the planar separator of the graph provided

straightline [graphfilename]

	Create a straight line drawing of a graph in graphviz format
	Visualize this by piping the output into plot.sh

	For exapmle, to visualize graphs/in,

	straightline graphs/in | plot.sh

plot.sh

	Produce and show a png image of the graphviz data coming from STDIN

planargen n e

	Outputs a randomly generated planar graph with n vertices and e edges
	Note that e must be <= 3n-6

unittest

	Run the suite of unit tests
