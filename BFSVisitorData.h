#pragma once
#include "typedefs.h"
#include "BFSVert.h"

struct BFSVisitorData
{
	std::map<vertex_t, std::set<vertex_t>> children;
	std::map<vertex_t, BFSVert>	       	   verts;
	uint				       			   num_levels;
	Graph const*				       		   g;
	vertex_t			       			   root; 

	BFSVisitorData(Graph const* g, vertex_t root);
	BFSVisitorData(BFSVisitorData const&) = default;

	void reset(Graph*); 
	bool is_tree_edge(edge_t) const; 
	uint edge_cost(edge_t, std::vector<vertex_t> const& cycle, Graph const&) const; 
	void print_costs  () const;
	void print_parents() const;
};

struct EdgeNotInVisitorData : std::exception
{
		edge_t e;
		Graph const* g;

		EdgeNotInVisitorData(edge_t e, Graph const* g) : e(e), g(g) {}
};

bool on_cycle(vertex_t, std::vector<vertex_t> const& cycle, Graph const&);
bool on_cycle(edge_t, std::vector<vertex_t> const& cycle, Graph const&);
