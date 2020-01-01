#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/breadth_first_search.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::property<boost::vertex_index_t, uint>, boost::property<boost::edge_index_t, uint>> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor                       vertex_t;


struct BFSVisitor : boost::default_bfs_visitor
{
};

int main()
{
        Graph g;
        vertex_t x;

        add_vertex(x, g);

        add_edge(x, x, g); // uncomment this line for workaround

        BFSVisitor v;
        breadth_first_search(g, x, boost::visitor(v));
}
