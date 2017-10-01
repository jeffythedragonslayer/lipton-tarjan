#pragma once
#include "typedefs.h"
#include "BFSVert.h"

struct BFSVisitorData
{
	std::map<VertDesc, std::set<VertDesc>> children;
	std::map<VertDesc, BFSVert>	       verts;
        uint				       num_levels;
        Graph*				       g;
        VertDesc			       root; 

	BFSVisitorData(Graph* g, VertDesc root);

	void reset(Graph*); 
	bool is_tree_edge(EdgeDesc) const; 
	uint edge_cost(EdgeDesc, std::vector<VertDesc> const& cycle, Graph const&) const; 
	void print_costs  () const;
	void print_parents() const;
};

bool on_cycle(VertDesc, std::vector<VertDesc> const& cycle, Graph const&);
bool on_cycle(EdgeDesc, std::vector<VertDesc> const& cycle, Graph const&);
