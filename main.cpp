#include "lipton-tarjan.h"
#include "strutil.h"
#include "graphutil.h"
#include <iostream> 
#include <fstream>
#include <csignal>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/copy.hpp>
#include <boost/bimap.hpp>
using namespace std;
using namespace boost;

int main(int argc, char* argv[])
{
        vector<string> fname;
        if( argc < 2 ){
                cout << "Usage: lt [filename]\n";
                return 0;
        } 
        for( uint i = 1; i < argc; ++i ) fname.push_back(argv[i]);

        for( auto& f : fname ){
                cout << "loading graph\n";
                auto g = load_graph(f, vmap);
                uint n = num_vertices(g);

                auto m = get(vertex_index, g);
                VertIter vi, vend;
                uint i = 0;
                for( tie(vi, vend) = vertices(g); vi != vend; ++vi ){
                        m[*vi] = i;
                        ++i;
                } 

                cout << "n: " << n << '\n';
                uint e = num_edges(g);

                cout << "starting lipton tarjan...\n";
                print_graph2(g);
                auto p = lipton_tarjan(g); 
        }
}
