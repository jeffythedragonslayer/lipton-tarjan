//=======================================================================
// Copyright 2015 - 2020 Jeff Linahan
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include "lipton-tarjan.h"
#include "typedefs.h"
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

#ifdef GRAPH_TYPE_LIST
                init_vert_propmap(g);
#endif

                uint n = num_vertices(g);

                cout << "n: " << n << '\n';
                uint e = num_edges(g);

                cout << "starting lipton tarjan...\n";
                print_graph(g);

		try { 
			Partition p = lipton_tarjan_separator(g);
			uint num_verts_finished = p.total_num_verts();
			cout << "Finished!\n";

                        p.print(&g);

                        BOOST_ASSERT(p.verify_edges(g));
			BOOST_ASSERT(p.verify_sizes(g));
			//p.print(vmap2);

			cout << "finished num verts: " << num_verts_finished << '\n';
		} catch (NotPlanarException e) {
			cout << "cannot finish lipton-tarjan because graph is not planar\n";
		} catch (NoNontreeEdgeException e) {
			cout << "cannot finish lipton-tarjan because I could not find a nontree edge out of " << e.num_edges << " edges\n";
		}
        }
}
