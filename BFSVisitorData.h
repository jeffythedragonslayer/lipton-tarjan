#pragma once
#include "typedefs.h"
#include "BFSVert.h"
#include <vector>

struct BFSVisitorData
{
	std::map<vertex_t, std::set<vertex_t>> children;
	std::map<vertex_t, BFSVert>	       	   verts;
	std::vector<edge_t>					   tree_edges;
	uint				       			   num_levels;
	Graph const*				           g;
	vertex_t			       			   root; 

	BFSVisitorData(Graph const* g, vertex_t root);
	BFSVisitorData(BFSVisitorData const&) = default;

	void reset(Graph*); 
	bool is_tree_edge(edge_t) const; 
	uint edge_cost(edge_t, std::vector<vertex_t> const& cycle, Graph const&) const; 
	void print_costs  () const;
	void print_parents() const;
	bool assert_data() const;
	void examine_edge(edge_t e);
	bool in_cc(edge_t e) const;
	std::vector<vertex_t> get_cycle(vertex_t v, vertex_t w, vertex_t ancestor) const;
	edge_t arbitrary_nontree_edge(Graph const&);
};

struct EdgeNotInVisitorData : std::exception
{
		edge_t e;
		Graph const* g;

		EdgeNotInVisitorData(edge_t e, Graph const* g) : e(e), g(g) {}
};

bool on_cycle(vertex_t, std::vector<vertex_t> const& cycle, Graph const&);
bool on_cycle(edge_t, std::vector<vertex_t> const& cycle, Graph const&);

struct NoNontreeEdgeException
{
		NoNontreeEdgeException(uint num_edges) : num_edges(num_edges) {}

		uint num_edges;
};
