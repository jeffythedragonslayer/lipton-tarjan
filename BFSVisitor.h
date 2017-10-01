#pragma once
#include "BFSVisitorData.h"
#include "typedefs.h"
#include <boost/lexical_cast.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/planar_canonical_ordering.hpp>
#include <boost/graph/is_straight_line_drawing.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp> 
#include <boost/graph/make_biconnected_planar.hpp>
#include <boost/graph/make_maximal_planar.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/config.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/pending/indirect_cmp.hpp>
#include <boost/range/irange.hpp> 
#include <boost/graph/copy.hpp>
#include <iostream>

struct BFSVisitor : boost::default_bfs_visitor
{
        BFSVisitorData& data;

        BFSVisitor(BFSVisitorData& data); 

	template<typename Edge, typename Graph> void tree_edge(Edge e, Graph const& g)
	{
		auto parent = source(e, g);
		auto child  = target(e, g);
		std::cout << "  tree edge " << parent << ", " << child << '\n';
		data.verts[child].parent = parent;
		data.verts[child].level  = data.verts[parent].level + 1;
		data.num_levels = std::max(data.num_levels, data.verts[child].level + 1);
		if( Graph::null_vertex() != parent ) data.children[parent].insert(child);

		VertDesc v = child;
		data.verts[v].descendant_cost = 1;
		//cout << "     vertex/descendant cost: ";
		//cout << v << '/'  << data.verts[v].descendant_cost << "   ";
		while( data.verts[v].level ){
			v =  data.verts[v].parent;
			++data.verts[v].descendant_cost;
			//cout << v << '/'  << data.verts[v].descendant_cost << "   ";
		} 
		//cout << '\n';
	} 
};
