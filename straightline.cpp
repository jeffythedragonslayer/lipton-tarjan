#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/planar_canonical_ordering.hpp>
#include <boost/graph/is_straight_line_drawing.hpp>
#include <boost/graph/make_connected.hpp>
#include <boost/graph/make_biconnected_planar.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/planar_face_traversal.hpp>
#include <boost/graph/chrobak_payne_drawing.hpp>
#include <boost/graph/make_maximal_planar.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <vector> 
#include <fstream>
#include <iostream> 
using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS, property<vertex_index_t, int>, property<edge_index_t, int>> Graph; 
typedef vector<vector<graph_traits<Graph>::edge_descriptor>>                                                embedding_storage_t;
typedef iterator_property_map<embedding_storage_t::iterator, property_map<Graph, vertex_index_t>::type>     embedding_t; 

struct face_counter : planar_face_traversal_visitor
{
        face_counter() : count(0) {}
        void begin_face() { ++count; }
        int count;
};

struct coord_t
{
        size_t x, y;
};

void make_max_planar(Graph& g)
{
        auto e_index = get(edge_index, g);
        graph_traits<Graph>::edges_size_type edge_count = 0;
        graph_traits<Graph>::edge_iterator ei, ei_end;
        for(tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) put(e_index, *ei, edge_count++);

        typedef vector< graph_traits<Graph>::edge_descriptor > vec_t;
        vector<vec_t> embedding(num_vertices(g));
        boyer_myrvold_planarity_test(boyer_myrvold_params::graph = g, boyer_myrvold_params::embedding = &embedding[0]);

        make_biconnected_planar(g, &embedding[0]);

        edge_count = 0;
        for(tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) put(e_index, *ei, edge_count++);

        boyer_myrvold_planarity_test(boyer_myrvold_params::graph = g, boyer_myrvold_params::embedding = &embedding[0]);

        make_maximal_planar(g, &embedding[0]);

        edge_count = 0;
        for(tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) put(e_index, *ei, edge_count++);

        boyer_myrvold_planarity_test(boyer_myrvold_params::graph = g, boyer_myrvold_params::embedding = &embedding[0]);

        face_counter count_visitor;
        planar_face_traversal(g, &embedding[0], count_visitor);
}

int main(int argc, char** argv)
{
        if( argc < 2 ){
                cerr << "Usage: straightline [filename]\n";
                return 1;
        }

        string fname(argv[1]);
        ifstream f(fname);
        if( !f ){
                cerr << "file " << fname << " not found!\n";
                return 1;
        } 

        string str;
        vector<pair<uint, uint>> edges;
        uint n = 0;
        while( getline(f, str) ){
                uint   colon = str.find(","); 
                string stra  = str.substr(0, colon); trim(stra);
                string strb  = str.substr(colon+1 ); trim(strb); 
                uint   a     = lexical_cast<uint>(stra);
                uint   b     = lexical_cast<uint>(strb);
                n = max(max(n, a), b);
                edges.push_back(make_pair(a, b));
        }
        
        Graph g(n); 
        for( auto& e : edges ) add_edge(e.first, e.second, g);
        make_max_planar(g);

        embedding_storage_t embedding_storage(num_vertices(g));
        embedding_t         embedding        (embedding_storage.begin(), get(vertex_index,g));

        boyer_myrvold_planarity_test(boyer_myrvold_params::graph = g, boyer_myrvold_params::embedding = embedding); 

        vector<graph_traits<Graph>::vertex_descriptor> ordering;
        planar_canonical_ordering(g, embedding, back_inserter(ordering));

        typedef vector<coord_t> 											      straight_line_drawing_storage_t;
        typedef iterator_property_map < straight_line_drawing_storage_t::iterator, property_map<Graph, vertex_index_t>::type> straight_line_drawing_t;

        straight_line_drawing_storage_t straight_line_drawing_storage (num_vertices(g));
        straight_line_drawing_t straight_line_drawing (straight_line_drawing_storage.begin(), get(vertex_index,g)); 

        chrobak_payne_straight_line_drawing(g, embedding, ordering.begin(), ordering.end(), straight_line_drawing); 

        graph_traits<Graph>::vertex_iterator vi, vi_end;
        cout << "graph G {\n";
        for( tie(vi,vi_end) = vertices(g); vi != vi_end; ++vi ){
                coord_t coord(get(straight_line_drawing,*vi));
                cout << *vi << "[pos=\"" << coord.x << ',' << coord.y << "!\"];\n";
        }
        for( auto& e : edges ) cout << e.first << "--" << e.second << " ;\n";
        cout << "}\n";
}
