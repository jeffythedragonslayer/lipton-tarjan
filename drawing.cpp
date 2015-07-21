// Copyright 2007 Aaron Windsor
// Distributed under the Boost Software License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at http://www..org/LICENSE_1_0.txt)
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp> 
#include <boost/graph/planar_canonical_ordering.hpp>
#include <boost/graph/is_straight_line_drawing.hpp>
#include <boost/graph/chrobak_payne_drawing.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp> 
#include <vector> 
#include <iostream>
using namespace std;
using namespace boost;

struct coord_t
{
	size_t x, y;
};


int main(int argc, char** argv)
{
	typedef adjacency_list<vecS, vecS, undirectedS, property<vertex_index_t, int>> graph;

	//Define the storage type for the planar embedding
	typedef vector<vector<graph_traits<graph>::edge_descriptor>> embedding_storage_t;
	typedef ::iterator_property_map<embedding_storage_t::iterator, property_map<graph, vertex_index_t>::type> embedding_t; 

	// Create the graph - a maximal planar graph on 7 vertices. The functions planar_canonical_ordering and chrobak_payne_straight_line_drawing both
	// require a maximal planar graph. If you start with a graph that isn't maximal planar (or you're not sure), you can use the functions
	// make_connected, make_biconnected_planar, and make_maximal planar in sequence to add a set of edges to any undirected planar graph to make it maximal planar.

	graph g(7);
	add_edge(0,1,g); add_edge(1,2,g); add_edge(2,3,g); add_edge(3,0,g);
	add_edge(3,4,g); add_edge(4,5,g); add_edge(5,6,g); add_edge(6,3,g);
	add_edge(0,4,g); add_edge(1,3,g); add_edge(3,5,g); add_edge(2,6,g);
	add_edge(1,4,g); add_edge(1,5,g); add_edge(1,6,g); 

	// Create the planar embedding
	embedding_storage_t embedding_storage(num_vertices(g));
	embedding_t embedding(embedding_storage.begin(), get(vertex_index,g));

	boyer_myrvold_planarity_test(boyer_myrvold_params::graph = g, boyer_myrvold_params::embedding = embedding); 

	// Find a canonical ordering
	vector<graph_traits<graph>::vertex_descriptor> ordering;
	planar_canonical_ordering(g, embedding, back_inserter(ordering)); 

	//Set up a property map to hold the mapping from vertices to coord_t's
	typedef vector<coord_t> 												straight_line_drawing_storage_t;
	typedef ::iterator_property_map<straight_line_drawing_storage_t::iterator, property_map<graph, vertex_index_t>::type> straight_line_drawing_t;

	straight_line_drawing_storage_t straight_line_drawing_storage(num_vertices(g));
	straight_line_drawing_t 	  straight_line_drawing(straight_line_drawing_storage.begin(), get(vertex_index,g));

	chrobak_payne_straight_line_drawing(g, embedding, ordering.begin(), ordering.end(), straight_line_drawing); // Compute the straight line drawing

	cout << "The straight line drawing is: \n";
	graph_traits<graph>::vertex_iterator vi, vi_end;
	for( ::tie(vi,vi_end) = vertices(g); vi != vi_end; ++vi ){
		coord_t coord(get(straight_line_drawing,*vi));
		cout << *vi << " -> (" << coord.x << ", " << coord.y << ")\n";
	}

	if( is_straight_line_drawing(g, straight_line_drawing) ) cout << "Is a plane drawing.\n";
	else 							   cout << "Is not a plane drawing.\n"; 
}
