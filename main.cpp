#include "lipton-tarjan.h"
#include <iostream> 
#include <fstream>
#include <csignal>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/planar_canonical_ordering.hpp>
#include <boost/graph/is_straight_line_drawing.hpp>
#include <boost/graph/chrobak_payne_drawing.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp> 
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/graph/graphviz.hpp>
using namespace std;
using namespace boost;

Graph load_graph(string fname)
{
        ifstream in(fname);
        if( !in ){
                cerr << "file \"in\" not found!\n";
                exit(0);
        }

        string str;
        vector<pair<uint, uint>> edges;
        while( getline(in, str) ){
                uint   colon = str.find(","); 
                string stra  = str.substr(0, colon); trim(stra);
                string strb  = str.substr(colon+1 ); trim(strb); 
                edges.push_back(make_pair(lexical_cast<uint>(stra), lexical_cast<uint>(strb)));
        } 
        Graph g(edges.size());
        vector<VertexDescriptor> v;
        VertexIterator vi, vi_end;
        for( tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi ) v.push_back(*vi);
        for( auto& e : edges ) add_edge(v[e.first], v[e.second], g);
        return g;
}

struct Coord
{
        size_t x, y;
};

typedef vector<Coord> StraightLineDrawingStorage; 

struct pos_writer
{
        Graph* g;

        pos_writer(Graph* g) : g(g) {}

        template <class VertexOrEdge>
        void operator() (ostream& out, const VertexOrEdge& v) const
        {
                int x = (*g)[v].x;
                int y = (*g)[v].y;
                out << "[pos=\"" << lexical_cast<int>(x) << ',' << lexical_cast<int>(y) << "!\"]";
        }
};

/*
void print_graph(Graph g)
{
        uint num_verts = num_vertices(g);
        graph_traits<Graph>::edge_iterator e, e_end;
        for( tie(e, e_end) = edges(g); e != e_end; ++e ){
                int src = source(*e, g);
                int tar = target(*e, g);
                cout << src << " <--> " << tar << '\n';
        }
}

void save_graph(Graph g, Embedding* embedding, vector<VertexDescriptor> ordering)
{ 
        typedef iterator_property_map<StraightLineDrawingStorage::iterator, property_map<Graph, vertex_index_t>::type> StraightLineDrawing;

        StraightLineDrawingStorage straight_line_drawing_storage(num_vertices(g));
        StraightLineDrawing        straight_line_drawing        (straight_line_drawing_storage.begin(), get(vertex_index,g));

        chrobak_payne_straight_line_drawing(g, *embedding, ordering.begin(), ordering.end(), straight_line_drawing); 

        ofstream f2("out_graph.txt");
        graph_traits<Graph>::vertex_iterator vi, vi_end;
        int i = 0;
        for( tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi ){
                Coord coord(get(straight_line_drawing,*vi));
                f2 << coord.x << ", " << coord.y << '\n'; 
                g[i].x = coord.x;
                g[i].y = coord.y;
                i++;
        }

        ofstream f("out_graph.dot");
        write_graphviz(f, g, pos_writer(&g));

}
*/

int main(int argc, char* argv[])
{
        vector<string> fname;
        if( argc < 2 ){
                cout << "Usage: lt [filename]\n";
                return 0;
        } 
        for( uint i = 1; i < argc; ++i ) fname.push_back(argv[i]);

        for( auto& f : fname ){
                auto g = load_graph(f);
                uint n = num_vertices(g);
                uint e = num_edges(g);

                print_graph(g);

                auto p = lipton_tarjan(g); 
                //save_graph(g, p.embedding, p.ordering);; 
        }
}
