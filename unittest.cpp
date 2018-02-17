#define BOOST_TEST_MODULE LiptonTarjanTest
#include "lipton-tarjan.h"
#include "strutil.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/test/included/unit_test.hpp>
#include <string> 
#include <algorithm>
using namespace std;
using namespace boost;

typedef adjacency_list<listS, listS, undirectedS, property<vertex_index_t, int>, property<edge_index_t, int>> Graph;

void check_partition_is_legal(string graphfile)
{
	cout << "Checking graph " << graphfile << "...\n";

	Graph g;
	fstream f(graphfile);

	vector<pair<uint, uint>> edges; 
	uint num_verts = 0;

	while( f ){
		uint edge1, edge2;
		f >> edge1;
		char comma;
		f >> comma;
		BOOST_CHECK(comma == ',');
		f >> edge2;

		num_verts = std::max(std::max(num_verts, edge1), edge2);
		edges.push_back(make_pair(edge1, edge2));
	}

	vector<vertex_t> verts(++num_verts);
	for( uint i = 0; i < num_verts; ++i ){
		verts[i] = add_vertex(g);
	}

	for( uint i = 0; i < edges.size(); ++i ){
		add_edge(verts[edges[i].first], verts[edges[i].second], g);
		cout << "added edge " << edges[i].first << ", " << edges[i].second << '\n';
	}

	//print_graph(g);
	auto partition = lipton_tarjan(g);
	//partition.print();
}

BOOST_AUTO_TEST_CASE( my_test )
{
	check_partition_is_legal("graphs/in");
}
