#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
using namespace std;
using namespace boost; 

int main(int argc, char** argv)
{ 
	// This program illustrates a simple use of boyer_myrvold_planar_embedding as a simple yes/no test for planarity.  

	typedef adjacency_list<vecS, vecS, undirectedS, property<vertex_index_t, int>> graph;

	graph K4(4);
	add_edge(0, 1, K4);
	add_edge(0, 2, K4);
	add_edge(0, 3, K4);
	add_edge(1, 2, K4);
	add_edge(1, 3, K4);
	add_edge(2, 3, K4);

	if( boyer_myrvold_planarity_test(K4) ) cout << "K4 is planar." << endl;
	else cout << "ERROR! K4 should have been recognized as planar!" << endl;

	graph K5(5);
	add_edge(0, 1, K5);
	add_edge(0, 2, K5);
	add_edge(0, 3, K5);
	add_edge(0, 4, K5);
	add_edge(1, 2, K5);
	add_edge(1, 3, K5);
	add_edge(1, 4, K5);
	add_edge(2, 3, K5);
	add_edge(2, 4, K5);

	// We've almost created a K5 - it's missing one edge - so it should still be planar at this point.

	if( boyer_myrvold_planarity_test(K5) ) cout << "K5 (minus an edge) is planar." << endl;
	else cout << "ERROR! K5 with one edge missing should" << " have been recognized as planar!" << endl;

	add_edge(3, 4, K5); // Now add the final edge...

	if( boyer_myrvold_planarity_test(K5) ) cout << "ERROR! K5 was recognized as planar!" << endl;
	else cout << "K5 is not planar." << endl;

	return 0;
}
