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

	cout << "starting lipton tarjan...\n";
	print_graph(g);

	Partition p = lipton_tarjan(g);

	// verify that neither a nor b is bigger than two thirds of the total and c is no bigger than 2*sqrt(2)*sqrt(n)
	cout << "verifying partition sizes\n";
	uint a_verts = p.a.size();
	uint b_verts = p.b.size();
	uint c_verts = p.c.size();
	uint n       = a_verts + b_verts + c_verts;
	cout << "n = " << n << '\n';

	BOOST_CHECK(a_verts <= 2*n/3);
	BOOST_CHECK(b_verts <= 2*n/3);
	BOOST_CHECK(c_verts <= 2*sqrt(2)*sqrt(n));

	// verify that no edge joins a vertex in partition A with a vertex in partition B
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

BOOST_AUTO_TEST_CASE( kuratowski )
{
		check_graph_is_nonplanar("graphs/kuratowski33");
		check_graph_is_nonplanar("graphs/kuratowski5");
}

BOOST_AUTO_TEST_CASE( empty_test )
{
		check_partition_is_legal("graphs/empty");
}

/*BOOST_AUTO_TEST_CASE( box2_test )
{
	check_partition_is_legal("graphs/box2");
}*/

/*BOOST_AUTO_TEST_CASE( huge_test )
{
	check_partition_is_legal("graphs/huge");
}*/

/*BOOST_AUTO_TEST_CASE( box3_test )
{
	check_partition_is_legal("graphs/box3");
}*/

/*BOOST_AUTO_TEST_CASE( delaunay_test )
{
	check_partition_is_legal("graphs/delaunay");
}*/

/*BOOST_AUTO_TEST_CASE( in2_test )
{
	check_partition_is_legal("graphs/in2");
}

BOOST_AUTO_TEST_CASE( in3_test )
{
	check_partition_is_legal("graphs/in3");
}

BOOST_AUTO_TEST_CASE( fourfingered_test )
{
	check_partition_is_legal("graphs/fourfingered");
}*/

/*BOOST_AUTO_TEST_CASE( insideout_test )
{
	check_partition_is_legal("graphs/insideout");
}*/

/*BOOST_AUTO_TEST_CASE( kuratowski33_test )
{
	check_graph_is_nonplanar("graphs/kuratowski33");
}

BOOST_AUTO_TEST_CASE( kuratowski5_test )
{
	check_graph_is_nonplanar("graphs/kuratowski5");
}*/

/*BOOST_AUTO_TEST_CASE( notk_test )
{
	check_partition_is_legal("graphs/notk", true);
}

BOOST_AUTO_TEST_CASE( rand_test )
{
	check_partition_is_legal("graphs/rand", true);
}

BOOST_AUTO_TEST_CASE( rand2_test )
{
	check_partition_is_legal("graphs/rand2", true);
}

BOOST_AUTO_TEST_CASE( rand3_test )
{
	check_partition_is_legal("graphs/rand3", true);
}

BOOST_AUTO_TEST_CASE( square_test )
{
	check_partition_is_legal("graphs/square", true);
}

BOOST_AUTO_TEST_CASE( tie_test )
{
	check_partition_is_legal("graphs/tie", true);
}

BOOST_AUTO_TEST_CASE( tri_test )
{
	check_partition_is_legal("graphs/tri", true);
}

BOOST_AUTO_TEST_CASE( two_test )
{
	check_partition_is_legal("graphs/two", true);
}

BOOST_AUTO_TEST_CASE( disconnected_test )
{
	check_partition_is_legal("graphs/disconnected", false);
}*/
