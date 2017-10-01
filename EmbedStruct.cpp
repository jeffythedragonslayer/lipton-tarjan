#include "EmbedStruct.h"
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <iostream>
using namespace std;
using namespace boost;

typedef boost::graph_traits<Graph>::vertex_iterator VertIter;

EmbedStruct::EmbedStruct(Graph* g) : g(g), storage(num_vertices(*g)), em(storage.begin()) 
{
	test_planar();
} 

bool EmbedStruct::test_planar() {return boyer_myrvold_planarity_test(boyer_myrvold_params::graph = *g, boyer_myrvold_params::embedding = em);}

void EmbedStruct::print()
{
	cout << "\n************** Embedding ************\n";
	VertIter vi, vend;
	for( tie(vi, vend) = vertices(*g); vi != vend; ++vi ){
		cout << "vert " << *vi << ": ";
		for( auto ei = em[*vi].begin(); ei != em[*vi].end(); ++ei ){
			auto src = source(*ei, *g);
			auto tar = target(*ei, *g);
			if( tar == *vi ) swap(src, tar);
			cout << tar << ' ';
		}
		cout << "\n"; 
	}
	cout << "*************************************\n";
}
