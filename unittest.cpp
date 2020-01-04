#define BOOST_TEST_MODULE LiptonTarjanTest
#include "typedefs.h"
#include "lipton-tarjan.h"
#include "strutil.h"
#include "graphutil.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/test/included/unit_test.hpp>
#include <boost/graph/graph_utility.hpp>
#include <string> 
#include <fstream>
#include <algorithm>
using namespace std;
using namespace boost;

void check_graph_is_nonplanar(string graphfile)
{ 
	cout << "loading graph\n";
	Graph g = load_graph(graphfile);
	init_vert_propmap(g);

	uint n = num_vertices(g);

	cout << "n: " << n << '\n';
	uint e = num_edges(g);

	cout << "starting lipton tarjan...\n";
	print_graph(g);

	try {
		Partition p = lipton_tarjan(g);
		BOOST_CHECK(false);
	} catch (NotPlanarException e){
		BOOST_CHECK(true);
	}
}

void check_partition_is_legal(string graphfile)
{
	fstream f(graphfile);
	if( !f.good() ){ 
		cerr << graphfile << " file does not exist!\n";
		BOOST_CHECK(false);
		return;
	}

	cout << "Checking graph " << graphfile << "...\n";

	Graph g = load_graph(graphfile);
	init_vert_propmap(g);

	cout << "starting lipton tarjan...\n";
	print_graph(g);
	uint n = num_vertices(g);

	Partition p = lipton_tarjan(g);


	uint total = p.total_num_verts();

	cout << "n = " << n << '\n';
	cout << "total = " << total << '\n';
	assert(n == total);

	assert(p.verify_sizes());
	assert(p.verify_edges(g));
}

BOOST_AUTO_TEST_CASE( kuratowski )
{
		check_graph_is_nonplanar("graphs/kuratowski33");
		check_graph_is_nonplanar("graphs/kuratowski5");
}

BOOST_AUTO_TEST_CASE( empty_test )
{
		check_partition_is_legal("graphs/empty");
}

/*BOOST_AUTO_TEST_CASE( star_test )
{
		check_partition_is_legal("graphs/star");
}*/

BOOST_AUTO_TEST_CASE( box2_test )
{
	check_partition_is_legal("graphs/box2");
}

/*BOOST_AUTO_TEST_CASE( huge_test )
{
	check_partition_is_legal("graphs/huge");
}*/

/*BOOST_AUTO_TEST_CASE( in3_test )
{
	check_partition_is_legal("graphs/in3");
}

BOOST_AUTO_TEST_CASE( fourfingered_test )
{
	check_partition_is_legal("graphs/fourfingered");
}*/

/*BOOST_AUTO_TEST_CASE( notk_test )
{
	check_partition_is_legal("graphs/notk", true);
}

BOOST_AUTO_TEST_CASE( rand_test )
{
	check_partition_is_legal("graphs/rand", true);
}*/

BOOST_AUTO_TEST_CASE( rand2_test )
{
	check_partition_is_legal("graphs/rand2");
}

/*BOOST_AUTO_TEST_CASE( rand3_test )
{
	check_partition_is_legal("graphs/rand3", true);
}*/

BOOST_AUTO_TEST_CASE( box3_test )
{
	check_partition_is_legal("graphs/box3");
}

BOOST_AUTO_TEST_CASE( notbb_test )
{
	check_partition_is_legal("graphs/notbb");
}

BOOST_AUTO_TEST_CASE( delaunay_test )
{
	check_partition_is_legal("graphs/delaunay");
}

BOOST_AUTO_TEST_CASE( in2_test )
{
	check_partition_is_legal("graphs/in2");
}

BOOST_AUTO_TEST_CASE( insideout_test )
{
	check_partition_is_legal("graphs/insideout");
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

BOOST_AUTO_TEST_CASE( aoeu_test )
{
	check_partition_is_legal("graphs/aoeu");
}

BOOST_AUTO_TEST_CASE( circular_node )
{
	check_partition_is_legal("graphs/one");
}

BOOST_AUTO_TEST_CASE( disconnected_test )
{
	check_partition_is_legal("graphs/disconnected");
}
