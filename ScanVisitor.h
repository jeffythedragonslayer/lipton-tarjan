#pragma once
#include "BFSVisitorData.h"
#include "typedefs.h"

struct ScanVisitor
{
	std::map<VertDesc, bool>*     		table;
        Graph*                        		g;
        VertDesc                      		x;
        uint                          		l0;
	std::set<std::pair<VertDesc, VertDesc>> edges_to_add, edges_to_delete;

        ScanVisitor(std::map<VertDesc, bool>* table, Graph*, VertDesc, uint l0); 
        void foundedge(VertDesc, EdgeDesc); 
        void finish(); 
        void scan_nonsubtree_edges(VertDesc, Graph const&, Embedding const&, BFSVisitorData const&);

};

