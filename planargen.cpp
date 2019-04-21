#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/ref.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <boost/graph/is_kuratowski_subgraph.hpp>
#include <boost/graph/connected_components.hpp>
#include <vector> 
#include <iostream>
using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS> Graph;
typedef graph_traits<Graph>::vertex_descriptor  vertex_t;
typedef graph_traits<Graph>::edge_iterator      EdgeIter;

int main(int argc, char** argv)
{ 
        if( argc < 3 ){
                cerr << "Usage: planargen n e\n";
                cerr << "   n - number of vertices\n";
                cerr << "   e - number of edges (<= 3n-6)\n";
                return 0;
        }

        int n = atoi(argv[1]);
        int e = atof(argv[2]);
        if( e > 3*n - 6 ){
                cerr << "Planr graph must have <= 3*n - 6 edges!\n";
                return 0;
        }

        Graph g(n); 
        uint v1, v2;
        for( uint i = 0; i < e; ++i ){ 

		v1 = i < n ?
		      i : // force all verts to be used at least once
		      rand() % n;

                do {
			v2 = rand() % n;
		} while( v1 == v2 );

                if( edge(v1, v2, g).second ) continue;

                auto g_tmp = g; 
                add_edge(v1, v2, g_tmp);

                if( boyer_myrvold_planarity_test(g_tmp) ){
                        g = g_tmp;
                        cout << v1 << ", " << v2 << '\n';
                        cerr << i*100.0/e << "% done\n";
                } else --i; 
        }

        vector<uint> component(num_vertices(g));
        uint num_cc = connected_components(g, &component[0]);

        vector<uint>::size_type i;
        cerr << "Total number of connected components: " << num_cc << endl;
}
