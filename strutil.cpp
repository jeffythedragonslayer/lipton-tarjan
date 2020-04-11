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
