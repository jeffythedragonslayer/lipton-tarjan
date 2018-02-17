#pragma once
#include "BFSVisitorData.h"
#include "typedefs.h"

struct ScanVisitor
{
	std::map<vertex_t, bool>*     		table;
        Graph*                        		g;
        vertex_t                      		x;
        uint                          		l0;
	std::set<std::pair<vertex_t, vertex_t>> edges_to_add, edges_to_delete;

        ScanVisitor(std::map<vertex_t, bool>* table, Graph*, vertex_t, uint l0); 
        void foundedge(vertex_t, edge_t); 
        void finish(); 
        void scan_nonsubtree_edges(vertex_t, Graph const&, Embedding const&, BFSVisitorData const&);
};
