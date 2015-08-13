#pragma once
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/property_map.hpp>
#include <vector>

typedef boost::adjacency_list<boost::listS, boost::listS, boost::undirectedS, boost::property<boost::vertex_index_t, int>> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor                                                                         VertexDescriptor;
typedef boost::graph_traits<Graph>::vertex_iterator                                                                           VertexIterator;
typedef boost::graph_traits<Graph>::edge_iterator                                                                             EdgeIterator;
typedef boost::graph_traits<Graph>::out_edge_iterator                                                                         OutEdgeIterator;
typedef boost::graph_traits<Graph>::edge_descriptor                                                                           EdgeDescriptor;
typedef boost::graph_traits<Graph>::edges_size_type                                                                           EdgesSizeType;
typedef std::vector<std::vector<EdgeDescriptor>>                                                                              EmbeddingStorage;
typedef boost::iterator_property_map<EmbeddingStorage::iterator, boost::property_map<Graph, boost::vertex_index_t>::type>     Embedding; 

struct Partition
{
        std::vector<VertexDescriptor> a, b, c;
}; 

Partition lipton_tarjan(Graph const& g);
