#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/erdos_renyi_generator.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <boost/graph/is_kuratowski_subgraph.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/ref.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <boost/graph/is_kuratowski_subgraph.hpp>
#include <iostream>
#include <vector> 
using namespace std;
using namespace boost;

typedef adjacency_list<>                                Graph;
typedef erdos_renyi_iterator<boost::minstd_rand, Graph> ERGen; 
typedef graph_traits<Graph>::vertex_descriptor          VertDesc;

int main(int argc, char* argv[])
{
        boost::minstd_rand gen; 
        if( argc < 3 ){
                cout << "Usage; erdos-renyi n p\n";
                cout << "   n - size of graph\n";
                cout << "   p - edge probability\n";
                return 0;
        }
        int   n = atoi(argv[1]);
        float p = atof(argv[2]);
        Graph g(ERGen(gen, n, p), ERGen(), n); 

        VertDesc v1, v2;
        typedef graph_traits<Graph>::edge_iterator edge_iter;
        pair<edge_iter, edge_iter> ep;
        edge_iter ei, ei_end;
        for( tie(ei, ei_end) = edges(g); ei != ei_end; ++ei ) cout << source(*ei, g) << ", " << target(*ei, g) << '\n';
}
