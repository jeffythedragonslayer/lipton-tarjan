#pragma once
#include "BFSVisitorData.h"
#include "EmbedStruct.h"
#include "typedefs.h"
#include <utility>

int levi_civita(uint i, uint j, uint k);

vertex_t			  get_common_ancestor(std::vector<vertex_t> const&, std::vector<vertex_t> const&); 
std::set<vertex_t> 	  get_neighbors(vertex_t, Graph const&);
std::set<vertex_t> 	  get_intersection(std::set<vertex_t> const&, std::set<vertex_t> const&);

enum InsideOutOn {INSIDE, OUTSIDE, ON};

InsideOutOn is_edge_inside_outside_or_on_cycle(edge_t, vertex_t common_vert, std::vector<vertex_t> const& cycle, Graph const&, Embedding const&);

struct FoundCircularNode
{
		FoundCircularNode(const vertex_t& e) : e(e) {}

		vertex_t e; 
};

struct CycleCost
{
        uint inside = 0;
		uint outside = 0;
};

CycleCost compute_cycle_cost(std::vector<vertex_t> const& cycle, Graph const&, BFSVisitorData const&, EmbedStruct const&);

EmbedStruct make_max_planar(Graph&);

void	  reset_vertex_indices(Graph&);
EdgeIndex reset_edge_index(Graph const&);

void kill_vertex(vertex_t, Graph&);

Graph load_graph(std::string const& fname);

vertex_t contract_vertices(vertex_t a, vertex_t b, Graph&);

bool vertex_exists(vertex_t x, Graph const&);
bool edge_exists(edge_t e, Graph const& g);
bool assert_verts(GraphCR, BFSVisitorData const&); 
void init_vert_propmap(Graph& g);