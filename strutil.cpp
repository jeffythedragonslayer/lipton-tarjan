#include "strutil.h"
#include <boost/lexical_cast.hpp>
using namespace std;
using namespace boost;

string to_string(edge_t e, Graph const& g)
{
        auto prop_map = get(vertex_index, g); // writing to this property map has side effects in the graph

        auto src = source(e, g);
        auto tar = target(e, g);
        stringstream ss;
        string str;
        ss << "(" << prop_map[src] << ", ";
        ss << prop_map[tar] << ")";
        return ss.str();
}

void print_cycle(vector<vertex_t> const& cycle, Graph const& g)
{
        auto prop_map = get(vertex_index, g); // writing to this property map has side effects in the graph
        cout << "cycle verts: ";
        for( auto& v : cycle ) cout << prop_map[v] << ' ';
        cout << '\n';
}

void print_graph_addresses(Graph const& g)
{ 
        cout << "\n**********************  Graph  ** print_graph_addresses ***\n"; 
        VertIter vi, vi_end;
        for( tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi ){
                cout << "vert " << *vi << '\n';
                OutEdgeIter ei, e_end;
                cout << "   hos edges ";
                for( tie(ei, e_end) = out_edges(*vi, g); ei != e_end; ++ei ){
                        auto src = source(*ei, g);
                        auto tar = target(*ei, g);
                        if( tar == *vi ) swap(src, tar);
                        cout << "( " << src << ' ' << tar << " )";
                }
                cout << '\n';
        }
        cout << "*********************************************************\n\n";
}

#if 0
void print_graph_nonboost(Graph const& g)
{ 
        cout << "\n**********************  Graph  ** print_graph_nonboost **********\n"; 
	map<vertex_t, uint> vmap;
	VertIter vi, vi_end;
        uint i = 0;
        for( tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi, ++i ){ 
		vmap.insert(make_pair(*vi, i));
        } 

	i = 0;
        for( tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi, ++i ){ 
                cout << "vert " << i;
                OutEdgeIter ei, e_end;
                cout << "   has edges ";
                for( tie(ei, e_end) = out_edges(*vi, g); ei != e_end; ++ei ){
                        auto src = source(*ei, g);
                        auto tar = target(*ei, g);
                        cout << (tar == *vi ? vmap[src] : vmap[tar]) << ' ';
                }
                cout << '\n';
        }
        cout << "*********************************************************\n\n";
}
#endif

#if 0
void print_edges(Graph const& g, Vert2UintMap const& vmap)
{
        cout << "\n**********************  Edges  **************************\n"; 
        EdgeIter ei, ei_end;
        for( tie(ei, ei_end) = edges(g); ei != ei_end; ++ei ){ 
                auto src = source(*ei, g);
                auto tar = target(*ei, g);
                cout << '(' << vmap.vert2uint.at(src) << ", " << vmap.vert2uint.at(tar) << ')';
		if( ei != ei_end ) cout << ", ";
        } 
        cout << "\n*********************************************************\n\n";
}
#endif
