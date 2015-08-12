#pragma once
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/property_map.hpp>
#include <vector>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::property<boost::vertex_index_t, int>, boost::property<boost::edge_index_t, int>> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor                                                                                                               VertexDescriptor;
typedef boost::graph_traits<Graph>::edge_iterator                                                                                                                   EdgeIterator;
typedef boost::graph_traits<Graph>::edge_descriptor                                                                                                                 EdgeDescriptor;
typedef boost::graph_traits<Graph>::edges_size_type                                                                                                                 EdgesSizeType;
typedef std::vector<std::vector<EdgeDescriptor>>                                                                                                                    EmbeddingStorage;
typedef boost::iterator_property_map<EmbeddingStorage::iterator, boost::property_map<Graph, boost::vertex_index_t>::type>                                           Embedding; 

struct Partitionh
{
        std::vector<VertexDescriptor> a, b, c;
}; 

Partitionh lipton_tarjan(Graph const& g);

void print_graph(Graph const& g);
