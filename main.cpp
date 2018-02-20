#include "lipton-tarjan.h"
#include <iostream> 
#include <fstream>
#include <csignal>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp> 
#include <boost/graph/copy.hpp>
#include <boost/bimap.hpp>
#include "strutil.h"
using namespace std;
using namespace boost;

extern map<vertex_t, uint> vert2uint;
extern map<uint, vertex_t> uint2vert;
extern bimap<vertex_t, uint> vu_bimap;

Graph load_graph(string fname)
{
        ifstream in(fname);
        if( !in ){
                cerr << "file \"in\" not found!\n";
                exit(1);
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
        Graph g(max_v+1);
        VertIter vi, vi_end;
        uint i = 0;
        for( tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi ){
                vert2uint[*vi] = i;
                uint2vert[i] = *vi;
		vu_bimap.insert({*vi, i});
                ++i;
        }
        for( auto& e : edges ){
                auto src = uint2vert[e.first];
                auto tar = uint2vert[e.second];
		//map<uint, vertex_t> m = vu_bimap.left;
                add_edge(src, tar, g);
        }

        vert2uint[Graph::null_vertex()] = -1;
	//vu_bimap.insert({Graph::null_vertex(), static_cast<uint>(-1)});
        return g;
}

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
                auto g = load_graph(f);
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
