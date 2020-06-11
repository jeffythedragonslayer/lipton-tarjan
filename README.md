# lipton-tarjan
Implementation of the 1977 linear time Lipton-Tarjan Planar Separator Theorem - finds a partition of a planar graph into sets A, B, and C.  C is the 'separator set' of nodes that partitions it roughly in half, such that there are no edges directly from any node in A to any node in B.  With a total of n nodes the size of C is bounded by 2 * sqrt(2) * sqrt(n), and the size of both A and B are bounded by 2*n/3.

This is intended to support further research into graph theory and applications such:

* Pebbling
* Boolean circuits and superconcentrators
* Maximal independent set problem
* Graph compression
* Distance structure calculations
* Polygon triangulation
* Jordan sorting

This codebase is built on top of the Boost Graph Library and is intended to further its goal of developing a resuable set of graph algorithms by providing a way to divide-and-conquer planar graphs in linear time.  If you want to code against this library, the key API function is called lipton_tarjan and key output data structure is a Partition struct.

To use it on the command line as an app, create graph data files as a list of pairs of nodes.  Several examples are in the graphs dir.  App usage:

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
	
Installation

	You will need to install graphviz to visualize the graphs
	Install boost in /usr/local/ and create a symbolic link to the directory,
	
	ln -s boost_1_72_0 boost
