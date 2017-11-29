#pragma once
#include "BFSVisitorData.h"
#include "EmbedStruct.h"
#include "typedefs.h"

int levi_civita(uint i, uint j, uint k);

VertDesc		common_ancestor(std::vector<VertDesc> const&, std::vector<VertDesc> const&); 
std::vector<VertDesc>	ancestors(VertDesc, BFSVisitorData const&);
std::vector<VertDesc>	get_cycle(VertDesc, VertDesc, VertDesc ancestor, BFSVisitorData const&);
std::vector<VertDesc>	get_cycle(VertDesc, VertDesc, BFSVisitorData const&); 
std::set<VertDesc> 	get_neighbors(VertDesc, Graph const&);
std::set<VertDesc> 	get_intersection(std::set<VertDesc> const&, std::set<VertDesc> const&);

enum InsideOutOn {INSIDE, OUTSIDE, ON};

InsideOutOn edge_inside_cycle(EdgeDesc, VertDesc common_vert, std::vector<VertDesc> const& cycle, Graph const&, Embedding const&);

EdgeDesc arbitrary_nontree_edge(Graph const&, BFSVisitorData const&);

struct CycleCost
{
        uint inside = 0;
	uint outside = 0;
};

CycleCost compute_cycle_cost(std::vector<VertDesc> const& cycle, Graph const&, BFSVisitorData const&, EmbedStruct const&);

void make_max_planar(Graph&);

void	  reset_vertex_indices(Graph&);
EdgeIndex reset_edge_index(Graph const&);

void kill_vertex(VertDesc, Graph&);
