#define BOOST_TEST_MODULE LiptonTarjanTest
#include "lipton-tarjan.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/test/included/unit_test.hpp>
#include <string> 
using namespace std;
using namespace boost;

void check_partition_is_legal(string graphfile)
{
	cout << "Checking graph " << graphfile << "...\n";

	typedef adjacency_list<listS, listS, undirectedS, property<vertex_index_t, int>, property<edge_index_t, int>> Graph;
	Graph g;
	fstream f(graphfile);

	vector<vertex_t> vs;

	for( uint i = 0; i < 100; ++i ){
		vs.push_back(add_vertex(g));
	}

	while( f ){
		int edge1, edge2;
		f >> edge1;
		char comma;
		f >> comma;
		BOOST_CHECK(comma == ',');
		f >> edge2;
		//v = edge1;
		add_edge(vs[edge1], vs[edge2], g);
	}

	//auto partition = lipton_tarjan(g);
	//partition.print();
}

BOOST_AUTO_TEST_CASE( my_test )
{
	check_partition_is_legal("graphs/in");
}

