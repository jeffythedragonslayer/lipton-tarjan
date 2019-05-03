#pragma once
#include "BFSVisitorData.h"
#include "EmbedStruct.h"
#include "Vert2UintMap.h"
#include "typedefs.h"

int levi_civita(uint i, uint j, uint k);

vertex_t		common_ancestor(std::vector<vertex_t> const&, std::vector<vertex_t> const&); 
std::vector<vertex_t>	ancestors(vertex_t, BFSVisitorData const&);
std::vector<vertex_t>	get_cycle(vertex_t, vertex_t, vertex_t ancestor, BFSVisitorData const&);
std::vector<vertex_t>	get_cycle(vertex_t, vertex_t, BFSVisitorData const&); 
std::set<vertex_t> 	get_neighbors(vertex_t, Graph const&);
std::set<vertex_t> 	get_intersection(std::set<vertex_t> const&, std::set<vertex_t> const&);

enum InsideOutOn {INSIDE, OUTSIDE, ON};

InsideOutOn edge_inside_outside_cycle(edge_t, vertex_t common_vert, std::vector<vertex_t> const& cycle, Graph const&, Embedding const&);

struct NoNontreeEdgeException
{
	NoNontreeEdgeException(uint num_edges) : num_edges(num_edges) {}

	uint num_edges;
};

edge_t arbitrary_nontree_edge(Graph const&, Vert2UintMap& vmap, BFSVisitorData const&);

struct CycleCost
{
        uint inside = 0;
	uint outside = 0;
};

CycleCost compute_cycle_cost(std::vector<vertex_t> const& cycle, Graph const&, BFSVisitorData const&, EmbedStruct const&);

void make_max_planar(Graph&);

void	  reset_vertex_indices(Graph&);
EdgeIndex reset_edge_index(Graph const&);

void kill_vertex(vertex_t, Graph&, Vert2UintMap& vmap);

Graph load_graph(std::string const& fname, Vert2UintMap& vmap);

void create_vmap_from_graph(Graph const& g, Vert2UintMap& vmap);
