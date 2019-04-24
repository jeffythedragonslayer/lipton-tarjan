#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/copy.hpp>
#include <iostream>
using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, uint32_t, float> AdjacencyList;

typedef AdjacencyList::vertex_descriptor VertexID;

    struct custom_property {
        //custom_property(uint32_t label = 0, float f = 0) : label(label), f(f) {}
        uint32_t label;
        float f;
    };

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, custom_property, float> AdjacencyListCustom;

struct vertex_copier {
    AdjacencyList& from;
    AdjacencyListCustom& to;

    void operator()(AdjacencyList::vertex_descriptor input, AdjacencyListCustom::vertex_descriptor output) const {
        to[output] = { from[input], 0.f };
    }
};

int main(int argc, char **argv) {

	cout << "copy test\n";
	AdjacencyList adj;
	VertexID id_0 = boost::add_vertex(0, adj);
	VertexID id_1 = boost::add_vertex(1, adj);
	VertexID id_2 = boost::add_vertex(2, adj);
	VertexID id_3 = boost::add_vertex(4, adj);
	boost::add_edge(id_0, id_1, 1.0f, adj);
	boost::add_edge(id_2, id_3, 2.0f, adj);

	AdjacencyListCustom adj_custom;
	//boost::copy_graph(adj, adj_custom, boost::vertex_copy(vertex_copier{adj, adj_custom}));
}
