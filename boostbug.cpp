#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <iostream>
using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::property<boost::vertex_index_t, uint>, boost::property<boost::edge_index_t, uint>> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor                       vertex_t;
typedef boost::graph_traits<Graph>::edge_descriptor                         edge_t;
typedef boost::graph_traits<Graph>::vertex_descriptor                       vertex_t;


struct CollectBFSVisitor : public boost::default_bfs_visitor
{
	virtual void initialize_vertex(vertex_t v, Graph const& g) {cout << "init vertex\n";};
	virtual void discover_vertex(vertex_t v, Graph const& g) {};
	virtual void examine_vertex(vertex_t v, Graph const& g) {};
	virtual void finish_vertex(vertex_t v, Graph const& g) {};
	virtual void black_target(edge_t, Graph const& g) {};
	virtual void gray_target(edge_t, Graph const& g) {};
	virtual void tree_target(edge_t, Graph const& g) {};
	virtual void tree_edge(edge_t, Graph const& g) {}; 
	virtual void non_tree_edge(edge_t, Graph const& g) {};
};

struct VertBuffer : boost::queue<vertex_t>
{
};

int main()
{
        Graph g;
        vertex_t x = add_vertex(g);
	auto indexmap = boost::get(boost::vertex_index, g);
	auto colormap = boost::make_vector_property_map<boost::default_color_type>(indexmap); 

	VertBuffer q;

        CollectBFSVisitor v;
        breadth_first_search(g, x, q, v, colormap);
}
