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

void verify_partition_sizes(Partition const& p)
{
	uint a_verts = p.a.size();
	uint b_verts = p.b.size();
	uint c_verts = p.c.size();
	uint n       = a_verts + b_verts + c_verts;

	BOOST_CHECK(a_verts <= 2*n/3);
	BOOST_CHECK(b_verts <= 2*n/3);
	BOOST_CHECK(c_verts <= 2*sqrt(2)*sqrt(n));
}

// verify that no edge joins a vertex in A with a vertex in B
void verify_partition_edges(Partition const& p, Graph const& g)
{
	pair<EdgeIter, EdgeIter> ep;
	EdgeIter ei, ei_end;
	for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei){
		auto v1 = source(*ei, g);
		auto v2 = target(*ei, g);
		bool v1_in_part_c = (find(STLALL(p.c), v1) != p.c.end());
		bool v2_in_part_c = (find(STLALL(p.c), v2) != p.c.end());
		BOOST_CHECK(!v1_in_part_c && !v2_in_part_c);

	}
}

void check_partition_is_legal(string graphfile)
{
	Graph g;
	fstream f(graphfile);
	if( !f.good() ){ 
		cerr << graphfile << " file does not exist!\n";
		BOOST_CHECK(false);
		return;
	}

	cout << "Checking graph " << graphfile << "...\n";

	vector<pair<uint, uint>> edges; 
	uint num_verts = 0;

	while( f ){
		uint edge1, edge2;
		f >> edge1;
		char comma;
		f >> comma;
		if ( f.eof() ) break;
		BOOST_CHECK(comma == ',');
		f >> edge2;

		num_verts = std::max(std::max(num_verts, edge1), edge2);
		edges.push_back(make_pair(edge1, edge2));
	}

	if( num_verts == 0 ) return;

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


	verify_partition_sizes(partition);
	verify_partition_edges(partition, g);
}

BOOST_AUTO_TEST_CASE( box2_test )
{
	check_partition_is_legal("graphs/box2");
}

BOOST_AUTO_TEST_CASE( box3_test )
{
	check_partition_is_legal("graphs/box3");
}

BOOST_AUTO_TEST_CASE( empty_test )
{
	check_partition_is_legal("graphs/empty");
}

BOOST_AUTO_TEST_CASE( huge_test )
{
	check_partition_is_legal("graphs/huge");
}

BOOST_AUTO_TEST_CASE( inm_test )
{
	check_partition_is_legal("graphs/in");
}

BOOST_AUTO_TEST_CASE( in2_test )
{
	check_partition_is_legal("graphs/in2");
}

BOOST_AUTO_TEST_CASE( in3_test )
{
	check_partition_is_legal("graphs/in3");
}

BOOST_AUTO_TEST_CASE( in4_test )
{
	check_partition_is_legal("graphs/in4");
}

BOOST_AUTO_TEST_CASE( insideout_test )
{
	check_partition_is_legal("graphs/insideout");
}

BOOST_AUTO_TEST_CASE( kuratowski33_test )
{
	check_partition_is_legal("graphs/kuratowski33");
}

BOOST_AUTO_TEST_CASE( kuratowski5_test )
{
	check_partition_is_legal("graphs/kuratowski5");
}

BOOST_AUTO_TEST_CASE( notk_test )
{
	check_partition_is_legal("graphs/notk");
}

BOOST_AUTO_TEST_CASE( rand_test )
{
	check_partition_is_legal("graphs/rand");
}

BOOST_AUTO_TEST_CASE( rand2_test )
{
	check_partition_is_legal("graphs/rand2");
}

BOOST_AUTO_TEST_CASE( rand3_test )
{
	check_partition_is_legal("graphs/rand3");
}

BOOST_AUTO_TEST_CASE( square_test )
{
	check_partition_is_legal("graphs/square");
}

BOOST_AUTO_TEST_CASE( tie_test )
{
	check_partition_is_legal("graphs/tie");
}

BOOST_AUTO_TEST_CASE( tri_test )
{
	check_partition_is_legal("graphs/tri");
}

BOOST_AUTO_TEST_CASE( two_test )
{
	check_partition_is_legal("graphs/two");
}

BOOST_AUTO_TEST_CASE( disconnected_test )
{
	check_partition_is_legal("graphs/disconnected");
}
