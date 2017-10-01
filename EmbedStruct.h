#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/property_map.hpp>
#include <vector>

typedef boost::adjacency_list<boost::listS, boost::listS, boost::undirectedS, boost::property<boost::vertex_index_t, int>, boost::property<boost::edge_index_t, int>>	Graph;
typedef boost::graph_traits<Graph>::edge_descriptor                                                                       						EdgeDesc;
typedef std::vector<std::vector<EdgeDesc>>                                                                                						EmbeddingStorage;
typedef boost::iterator_property_map<EmbeddingStorage::iterator, boost::property_map<Graph, boost::vertex_index_t>::type> 						Embedding; 

struct EmbedStruct
{
        EmbeddingStorage storage;
        Embedding        em;
        Graph*           g;

        EmbedStruct(Graph*); 

        bool test_planar(); 
        void print();
};
