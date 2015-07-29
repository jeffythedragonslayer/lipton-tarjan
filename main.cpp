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
using namespace std;
using namespace boost;

Graph load_graph()
{
        ifstream in("in");
        if( !in ){
                cerr << "file \"in\" not found!\n";
                exit(0);
        }

        string str;
	Graph g;
        while( getline(in, str) ){
                uint   colon = str.find(","); 
                string stra  = str.substr(0, colon); trim(stra);
                string strb  = str.substr(colon+1 ); trim(strb); 
                uint   a     = lexical_cast<uint>(stra);
                uint   b     = lexical_cast<uint>(strb); 
                add_edge(a, b, g);
        } 
        return g;
}

struct Coord
{
        size_t x, y;
};

typedef vector<Coord> StraightLineDrawingStorage; 

void save_graph(Graph g, Embedding* embedding, vector<VertexDescriptor> ordering)
{ 
        typedef iterator_property_map<StraightLineDrawingStorage::iterator, property_map<Graph, vertex_index_t>::type> StraightLineDrawing;

        StraightLineDrawingStorage straight_line_drawing_storage(num_vertices(g));
        StraightLineDrawing        straight_line_drawing        (straight_line_drawing_storage.begin(), get(vertex_index,g));

        chrobak_payne_straight_line_drawing(g, *embedding, ordering.begin(), ordering.end(), straight_line_drawing); 

        graph_traits<Graph>::vertex_iterator vi, vi_end;
        ofstream f("out");
        for( tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi ){
                Coord coord(get(straight_line_drawing,*vi));
                f << *vi << " -> (" << coord.x << ", " << coord.y << ")\n";
        }
}


int main()
{
        auto g = load_graph();
	auto p = lipton_tarjan(g); 
	save_graph(g, p.embedding, p.ordering);
}
