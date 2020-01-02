#include "lipton-tarjan.h"
#include "strutil.h"
#include "graphutil.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/copy.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/bimap.hpp>
#include <iostream> 
#include <fstream>
#include <csignal>
#include <utility>
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

        for( string& f : fname ){
                cout << "loading graph\n";

                Graph g = load_graph(f);

                /*auto prop_map = get(vertex_index, g); // writing to this property map has side effects in the graph
                VertIter vi, vend;
                uint i = 0;
                for( tie(vi, vend) = vertices(g); vi != vend; ++vi ){
                        //prop_map[*vi] = i;
			cout << "prop map " << *vi << " == " << i << '\n';
                        ++i;
                }*/

                uint n = num_vertices(g);


                cout << "n: " << n << '\n';
                uint e = num_edges(g);

                cout << "starting lipton tarjan...\n";
                print_graph(g);

		try { 
			Partition p = lipton_tarjan(g);
			uint num_verts_finished = p.total_num_verts();
			cout << "Finished!\n";

                        p.print();
			//p.print(vmap2);

			cout << "finished num verts: " << num_verts_finished << '\n';
		} catch (NotPlanarException e) {
			cout << "cannot finish lipton-tarjan because graph is not planar\n";
		} catch (NoNontreeEdgeException e) {
			cout << "cannot finish lipton-tarjan because I could not find a nontree edge out of " << e.num_edges << " edges\n";
		}
        }
}
