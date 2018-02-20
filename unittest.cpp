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
}

void verify_partition_edges(Partition const& p)
{
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
	verify_partition_edges(partition);
}

//BOOST_AUTO_TEST_CASE( box2my_test )
//{
	//check_partition_is_legal("graphs/box2");
//}

//BOOST_AUTO_TEST_CASE( box3my_test )
//{
	//check_partition_is_legal("graphs/box3");
//}

BOOST_AUTO_TEST_CASE( emptymy_test )
{
	check_partition_is_legal("graphs/empty");
}

//BOOST_AUTO_TEST_CASE( hugemy_test )
//{
//	check_partition_is_legal("graphs/huge");
//}

//BOOST_AUTO_TEST_CASE( inmy_test )
//{
//	check_partition_is_legal("graphs/in");
//}

/*BOOST_AUTO_TEST_CASE( in2my_test )
{
	check_partition_is_legal("graphs/in2");
}

BOOST_AUTO_TEST_CASE( in3my_test )
{
	check_partition_is_legal("graphs/in3");
}

BOOST_AUTO_TEST_CASE( in4my_test )
{
	check_partition_is_legal("graphs/in4");
}

BOOST_AUTO_TEST_CASE( insideoutmy_test )
{
	check_partition_is_legal("graphs/insideout");
}*/

BOOST_AUTO_TEST_CASE( kuratowski33my_test )
{
	check_partition_is_legal("graphs/kuratowski33");
}

BOOST_AUTO_TEST_CASE( kuratowski5my_test )
{
	check_partition_is_legal("graphs/kuratowski5");
}

BOOST_AUTO_TEST_CASE( notkmy_test )
{
	check_partition_is_legal("graphs/notk");
}

/*BOOST_AUTO_TEST_CASE( randmy_test )
{
	check_partition_is_legal("graphs/rand");
}

BOOST_AUTO_TEST_CASE( rand2my_test )
{
	check_partition_is_legal("graphs/rand2");
}

BOOST_AUTO_TEST_CASE( rand3my_test )
{
	check_partition_is_legal("graphs/rand3");
}*/

BOOST_AUTO_TEST_CASE( squaremy_test )
{
	check_partition_is_legal("graphs/square");
}

BOOST_AUTO_TEST_CASE( tiemy_test )
{
	check_partition_is_legal("graphs/tie");
}

BOOST_AUTO_TEST_CASE( trimy_test )
{
	check_partition_is_legal("graphs/tri");
}

BOOST_AUTO_TEST_CASE( twomy_test )
{
	check_partition_is_legal("graphs/two");
}
