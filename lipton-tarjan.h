#pragma once
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <vector>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::property<boost::vertex_index_t, int>>  Graph;
typedef std::vector<std::vector<boost::graph_traits<Graph>::edge_descriptor>>                                             EmbeddingStorage;
typedef boost::iterator_property_map<EmbeddingStorage::iterator, boost::property_map<Graph, boost::vertex_index_t>::type> Embedding; 
typedef boost::graph_traits<Graph>::vertex_descriptor                                                                     VertexDescriptor;

struct Partition
{
        Graph a, b;
        Graph c;

        std::vector<VertexDescriptor> ordering;
        EmbeddingStorage*             embedding_storage;
        Embedding*                    embedding;
        uint                          costly_component;
};


Partition lipton_tarjan(Graph g);
