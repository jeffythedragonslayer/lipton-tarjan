#pragma once
#include "BFSVert.h"
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/property_map.hpp>
#include <vector>

typedef boost::adjacency_list<boost::listS, boost::listS, boost::undirectedS, boost::property<boost::vertex_index_t, int>, boost::property<boost::edge_index_t, int>> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor                                                                                                                 VertDesc;
typedef boost::graph_traits<Graph>::vertex_iterator                                                                                                                   VertIter;
typedef std::map<VertDesc, size_t>                                                                                                                                    VertDescMap;
typedef boost::graph_traits<Graph>::edge_iterator                                                                                                                     EdgeIter;
typedef boost::graph_traits<Graph>::edge_descriptor                                                                                                                   EdgeDesc;
typedef boost::graph_traits<Graph>::out_edge_iterator                                                                                                                 OutEdgeIter;
typedef boost::graph_traits<Graph>::edge_descriptor                                                                                                                   EdgeDesc;
typedef boost::graph_traits<Graph>::edges_size_type                                                                                                                   EdgesSizeType;
typedef std::vector<std::vector<EdgeDesc>>                                                                                                                            EmbeddingStorage;
typedef boost::iterator_property_map<EmbeddingStorage::iterator, boost::property_map<Graph, boost::vertex_index_t>::type>                                             Embedding; 
typedef boost::property_map<Graph, boost::edge_index_t>::type                                                                                                         EdgeIndex; 

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
