#include "EmbedStruct.h"
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <iostream>
using namespace std;
using namespace boost;

typedef boost::graph_traits<Graph>::vertex_iterator VertIter;

bool test_planar_workaround(Embedding& em, Graph* g)
{
	if( num_vertices(*g) < 5) return true;
	// boyer_myrvold_planarity_test sometimes crashes when given strange graphs 

        // workaround for https://github.com/boostorg/graph/issues/195 
        VertIter vit, vjt;
        tie(vit, vjt) = vertices(*g);
        for( VertIter next = vit; vit != vjt; ++vit){
                if( 0 == in_degree(*vit, *g) + out_degree(*vit, *g) ){
                        add_edge(*vit, *vit, *g);
                }
        }
 
	return boyer_myrvold_planarity_test(boyer_myrvold_params::graph = *g, boyer_myrvold_params::embedding = em);
}


EmbedStruct ctor_workaround(Graph* g)
{
	EmbedStruct es;
	es.g = g;
	es.storage = EmbeddingStorage(num_vertices(*g));
	es.em = es.storage.begin();
	bool p = test_planar_workaround(es.em, g);
	return es;

}

EmbedStruct::EmbedStruct(Graph const* g) : g(g), storage(num_vertices(*g)), em(storage.begin()) 
{
		cout << "EmbedStruct ctor\n";
		bool p = test_planar();
		if (!p){
				cout << "this graph is not planar\n";
		}
} 

bool EmbedStruct::test_planar() const
{
	// boyer_myrvold_planarity_test sometimes crashes when given strange graphs 
	return boyer_myrvold_planarity_test(boyer_myrvold_params::graph = *g, boyer_myrvold_params::embedding = em);
}

void EmbedStruct::print() const
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
