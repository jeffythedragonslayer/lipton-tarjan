#include "lipton-tarjan.h"
#include "strutil.h"
#include "Vert2UintMap.h"
#include <iostream> 
#include <fstream>
#include <csignal>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp> 
#include <boost/graph/copy.hpp>
#include <boost/bimap.hpp>
using namespace std;
using namespace boost;

struct FileNotFound {};

void load_graph(string fname, Graph& g, Vert2UintMap& vmap)
{
        ifstream in(fname);
        if( !in ){
                cerr << "file \"in\" not found!\n";
		throw FileNotFound();
        }

        string str;
        vector<pair<uint, uint>> edges;
        uint max_v = 0;
        while( getline(in, str) ){
                uint   colon = str.find(","); 
                string stra  = str.substr(0, colon); trim(stra);
                string strb  = str.substr(colon+1 ); trim(strb); 
                uint   a     = lexical_cast<uint>(stra);
                uint   b     = lexical_cast<uint>(strb);
                max_v = max(max(max_v, a), b);
                edges.push_back(make_pair(a, b));
        } 

	vector<vertex_t> verts(max_v+1);
	for( uint i = 0; i < max_v+1; ++i){
		verts.push_back(add_vertex(g));
	}

        VertIter vi, vi_end;
        uint i = 0;
        for( tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi ){
                vmap.vert2uint[*vi] = i;
                vmap.uint2vert[i] = *vi;
		vmap.vu_bimap.insert({*vi, i});
                ++i;
        }

        for( auto& e : edges ){
                auto src = vmap.uint2vert[e.first];
                auto tar = vmap.uint2vert[e.second];
		//map<uint, vertex_t> m = vu_bimap.left;
                add_edge(src, tar, g);
        }

        vmap.vert2uint[Graph::null_vertex()] = -1;
	vmap.vu_bimap.insert({Graph::null_vertex(), static_cast<uint>(-1)});
}

int main(int argc, char* argv[])
{
        //vector<string> fname;
        string f = argv[1];
        if( argc < 2 ){
                cout << "Usage: lt [filename]\n";
                return 0;
        } 
        //for( uint i = 1; i < argc; ++i ) fname.push_back(argv[i]);

	Graph g, g_copy;
	Vert2UintMap vmap, vmap_copy;

        //for( auto& f : fname ){
                cout << "loading graph\n";
                load_graph(f, g, vmap);
                cout << "loading graph copy\n";
		load_graph(f, g_copy, vmap_copy);
                uint n = num_vertices(g);

                /*auto m = get(vertex_index, g);
                VertIter vi, vend;
                uint i = 0;
                for( tie(vi, vend) = vertices(g); vi != vend; ++vi ){
                        m[*vi] = i;
                        ++i;
                } */

                cout << "n: " << n << '\n';
                uint e = num_edges(g);

                cout << "starting lipton tarjan...\n";

		cout << "vmap: ";
		vmap.print();
		cout << "vmap_copy: ";
		vmap_copy.print();
                auto p = lipton_tarjan(g, g_copy, vmap, vmap_copy);
        //}
}
