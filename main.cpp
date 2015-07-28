#include "lipton-tarjan.h"
#include <iostream> 
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/planar_canonical_ordering.hpp>
#include <boost/graph/is_straight_line_drawing.hpp>
#include <boost/graph/chrobak_payne_drawing.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp> 
using namespace std;
using namespace boost;

struct coord_t
{
        size_t x, y;
};

void draw(Graph g, Embedding* embedding)
{ 
        typedef vector<coord_t> straight_line_drawing_storage_t;
        typedef boost::iterator_property_map<straight_line_drawing_storage_t::iterator, property_map<Graph, vertex_index_t>::type> straight_line_drawing_t;

        vector<graph_traits<Graph>::vertex_descriptor> ordering;
        planar_canonical_ordering(g, *embedding, back_inserter(ordering)); 

        straight_line_drawing_storage_t straight_line_drawing_storage(num_vertices(g));
        straight_line_drawing_t         straight_line_drawing(straight_line_drawing_storage.begin(), get(vertex_index,g));

        chrobak_payne_straight_line_drawing(g, *embedding, ordering.begin(), ordering.end(), straight_line_drawing); 

        cout << "The straight line drawing is: \n";
        graph_traits<Graph>::vertex_iterator vi, vi_end;
        for( tie(vi,vi_end) = vertices(g); vi != vi_end; ++vi ){
                coord_t coord(get(straight_line_drawing,*vi));
                cout << *vi << " -> (" << coord.x << ", " << coord.y << ")\n";
        }

        cout << (is_straight_line_drawing(g, straight_line_drawing) ? "Is a plane drawing.\n" : "Is not a plane drawing.\n");
}


int main()
{
	Graph g;
        add_edge(0,1,g); add_edge(1,2,g); add_edge(2,3,g); add_edge(3,0,g); add_edge(3,4,g); add_edge(4,5,g); add_edge(5,6,g); add_edge(6,3,g); add_edge(0,4,g); add_edge(1,3,g);
        add_edge(3,5,g); add_edge(2,6,g); add_edge(1,4,g); add_edge(1,5,g); add_edge(1,6,g); 

	auto p = lipton_tarjan(g);

	draw(g, p.embedding);
}
