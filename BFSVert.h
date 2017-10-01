#pragma once
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/property_map.hpp>
#include <vector>

typedef boost::adjacency_list<boost::listS, boost::listS, boost::undirectedS, boost::property<boost::vertex_index_t, int>, boost::property<boost::edge_index_t, int>> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor                                                                                                                 VertDesc;

struct BFSVert
{
        BFSVert() : parent(Graph::null_vertex()), level(0), descendant_cost(0) {}

        VertDesc parent;
        uint     level, descendant_cost;
};

