#pragma once
#include "style.h"
#undef self
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <vector>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::property<boost::vertex_index_t, int>>      bgraph;
typedef std::vector<std::vector<boost::graph_traits<bgraph>::edge_descriptor>>                                                embedding_storage_t;
typedef boost::iterator_property_map<embedding_storage_t::iterator, boost::property_map<bgraph, boost::vertex_index_t>::type> embedding_t; 

struct Partition
{
        bgraph a, b;
        bgraph c;

        embedding_storage_t* embedding_storage;
        embedding_t*         embedding;
};

Partition lipton_tarjan(bgraph g);
