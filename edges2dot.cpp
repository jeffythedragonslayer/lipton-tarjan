#include <boost/config.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/planar_canonical_ordering.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/chrobak_payne_drawing.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp> 
#include <boost/graph/planar_canonical_ordering.hpp>
#include <boost/graph/is_straight_line_drawing.hpp>
#include <boost/graph/chrobak_payne_drawing.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;
using namespace boost; 

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::property<boost::vertex_index_t, int>, boost::property<boost::edge_index_t, int>> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor                                                                                                                 VertDesc;
typedef boost::graph_traits<Graph>::vertex_iterator                                                                                                                   VertIter;
typedef std::map<VertDesc, size_t>                                                                                                                                    VertDescMap;
typedef boost::graph_traits<Graph>::edge_iterator                                                                                                                     EdgeIter;
typedef boost::graph_traits<Graph>::edge_descriptor                                                                                                                   EdgeDesc;
typedef boost::graph_traits<Graph>::out_edge_iterator                                                                                                                 OutEdgeIter;
typedef boost::graph_traits<Graph>::edge_descriptor                                                                                                                   EdgeDesc;
typedef boost::graph_traits<Graph>::edges_size_type                                                                                                                   EdgesSizeType;
typedef std::vector<std::vector<EdgeDesc>>                                                                                                                            EmbeddingStorage;
typedef boost::iterator_property_map<EmbeddingStorage::iterator, boost::property_map<Graph, boost::vertex_index_t>::type>                                             Embedding; 
typedef boost::property_map<Graph, boost::edge_index_t>::type                                                                                                         EdgeIndex; 
/*
struct pos_writer
{
        Graph* gg;

        template <class VertexOrEdge>
        void operator() (ostream& out, const VertexOrEdge& v) const
        {
                Graph& g = *gg;
                int x = g[v].x;
                int y = g[v].y;
                out << "[pos=\"" << lexical_cast<int>(x) << ',' << lexical_cast<int>(y) << "!\"]";
        }
};

struct Coord
{
        size_t x, y;
};

typedef vector<Coord> StraightLineDrawingStorage; 

void save_graph(Graph& g, Embedding* embedding, vector<VertDesc> ordering)
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
                //g[i].x = coord.x;
                //g[i].y = coord.y;
                i++;
        }

        ofstream f("out_graph.dot");
        //write_graphviz(f, g, pos_writer()); 
}
*/

Graph load_graph(string fname)
{
        ifstream in(fname);
        if( !in ){
                cerr << "file \"in\" not found!\n";
                exit(0);
        }

        uint n = 24;
        Graph g(n);

        string str;
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

/*
int main(int argc, char* argv[])
{
        if( argc < 2 ){
                cout << "Usage: lt [filename]\n";
                return 0;
        } 

        string fname = argv[1];

        cout << "loading graph\n";
        auto g = load_graph(fname);

        EmbeddingStorage storage(num_vertices(g));
        Embedding embedding(storage.begin(), get(vertex_index, g));
        boyer_myrvold_planarity_test(g, &embedding[0]);

        typedef vector<VertDesc> ordering_storage_t;
        ordering_storage_t ordering;
        planar_canonical_ordering(g, &embedding[0], back_inserter(ordering));

        save_graph(g, &embedding, ordering);
}
*/

using namespace boost;

//a class to hold the coordinates of the straight line embedding
struct coord_t
{
  std::size_t x;
  std::size_t y;
};


int main(int argc, char** argv)
{
  typedef adjacency_list < vecS, vecS, undirectedS, property<vertex_index_t, int> > graph; 

  //Define the storage type for the planar embedding
  typedef std::vector< std::vector< graph_traits<graph>::edge_descriptor > > embedding_storage_t;
  typedef boost::iterator_property_map < embedding_storage_t::iterator, property_map<graph, vertex_index_t>::type > embedding_t;



  // Create the graph - a maximal planar graph on 7 vertices. The functions
  // planar_canonical_ordering and chrobak_payne_straight_line_drawing both
  // require a maximal planar graph. If you start with a graph that isn't
  // maximal planar (or you're not sure), you can use the functions
  // make_connected, make_biconnected_planar, and make_maximal planar in
  // sequence to add a set of edges to any undirected planar graph to make
  // it maximal planar .
  
  string fname = argv[1]; 
  cout << "loading graph\n";
  auto g = load_graph(fname);

  // Create the planar embedding
  embedding_storage_t embedding_storage(num_vertices(g));
  embedding_t embedding(embedding_storage.begin(), get(vertex_index,g));

  boyer_myrvold_planarity_test(boyer_myrvold_params::graph = g, boyer_myrvold_params::embedding = embedding); 

  // Find a canonical ordering
  std::vector<graph_traits<graph>::vertex_descriptor> ordering;
  planar_canonical_ordering(g, embedding, std::back_inserter(ordering));

  //Set up a property map to hold the mapping from vertices to coord_t's
  typedef std::vector< coord_t > straight_line_drawing_storage_t;
  typedef boost::iterator_property_map < straight_line_drawing_storage_t::iterator, property_map<graph, vertex_index_t>::type > straight_line_drawing_t;

  straight_line_drawing_storage_t straight_line_drawing_storage (num_vertices(g));
  straight_line_drawing_t straight_line_drawing (straight_line_drawing_storage.begin(), get(vertex_index,g));

  // Compute the straight line drawing
  chrobak_payne_straight_line_drawing(g, embedding, ordering.begin(), ordering.end(), straight_line_drawing); 

  std::cout << "The straight line drawing is: " << std::endl;
  graph_traits<graph>::vertex_iterator vi, vi_end;
  for(tie(vi,vi_end) = vertices(g); vi != vi_end; ++vi) {
      coord_t coord(get(straight_line_drawing,*vi));
      std::cout << *vi << " -> (" << coord.x << ", " << coord.y << ")" << std::endl;
    }

  // Verify that the drawing is actually a plane drawing
  if (is_straight_line_drawing(g, straight_line_drawing)) std::cout << "Is a plane drawing." << std::endl;
  else std::cout << "Is not a plane drawing." << std::endl;

  return 0;
}
