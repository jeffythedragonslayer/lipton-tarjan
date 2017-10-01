#pragma once
#include "BFSVisitorData.h"
#include <map>
#include <set>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/property_map.hpp>
typedef boost::adjacency_list<boost::listS, boost::listS, boost::undirectedS, boost::property<boost::vertex_index_t, int>, boost::property<boost::edge_index_t, int>> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor 														      VertDesc;
typedef boost::graph_traits<Graph>::edge_descriptor                                                                                                                   EdgeDesc;
typedef boost::iterator_property_map<EmbeddingStorage::iterator, boost::property_map<Graph, boost::vertex_index_t>::type>                                             Embedding; 
typedef std::vector<std::vector<EdgeDesc>>                                                                                                                            EmbeddingStorage;

struct ScanVisitor
{
	std::map<VertDesc, bool>*     		table;
        Graph*                        		g;
        VertDesc                      		x;
        uint                          		l0;
	std::set<std::pair<VertDesc, VertDesc>> edges_to_add, edges_to_delete;

        ScanVisitor(std::map<VertDesc, bool>* table, Graph*, VertDesc, int l0); 
        void foundedge(VertDesc, EdgeDesc); 
        void finish(); 
        void scan_nonsubtree_edges(VertDesc, Graph const&, Embedding const&, BFSVisitorData const&);

};

