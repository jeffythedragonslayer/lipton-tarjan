#include "lipton-tarjan.h"
#include <iostream>
#include <vector>
#undef self
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

// a class to hold the coordinates of the straight line embedding
struct coord_t
{
        size_t x, y;
}; 

void step1()
{
        // find a planar embedding of G and construct a representation for it of the kind described above
        // Boost Boyer-Myrvold 2004
}

bool step2()
{
        // Find the connected components of G and determine the cost of each one.
        if( false /* none has cost exceeding 2/3 */ ){
                // construct the paritition as described in the proof of Theorem 4.
                return true;
        }
        return false;
}

void step3()
{
        // Find a breadth-first spanning tree of the most costly component.
        // Compute the level of each vertex and the # of of verts L(l) in each level l.
}

void step4()
{
        // Find level l1 such that the total cost of levels 0 through l1 - 1 does not exceed 1/2, but the total cost of levels 0 through l1 does exceed 1/2.
        // Let k = # of vertices in levels 0 through l1.
}

void step5()
{
        // Find highest level l0 <= l1,     such that L(l0) + 2(l1-l0)       <= 2*sqrt(k)
        // Find lowest  level l2 >= l1 + 1, such that L(l2) + 2(l2 - l1 - 1) <= 2*sqrt(n-k).
}

void step6shrink()
{
        int table[99];
        // Scan the edges incident to this tree clockwise around the tree.
                // When scanning an edge(v,w) with v in the tree...
                int w = 0;
                if ( !table[w] ){
                        table[w] = true;
                        // construct an edge(x, w)
                }
                // delete edge(v, w).
}

void step6()
{
        // Delete all the vertices on level l2 and above.
        // Construct a new vertex x to represent all vertices on level 0 through l0.

        // Construct a boolean table with one entry per vertex.
        // Initialize to true  the entry for each vertex on levels 0      through l0.           (The vertices on these levels correspond to a subtree of the breadth-first spanning tree generated in step 3.)
        // "          "  false "   "     "   "    "      "  "      l0 + 1 "       l2 - 1.  

        step6shrink();
        // The result of this step is a planar representation of the shruken graph to which Lemma 2 is applied.
}

void step7()
{
        // Construct a breadth-first spanning tree rooted at x in the new graph.  (by modifying the breadth-first spanning tree constructed in Step 3.)
        // for each vertex v
                // record the parent of v in the tree
                // record the total cost of all descendants of v including v itself.
        // Make all faces of the new graph into triangles by scanning the boundary of each face and adding (nontree) edges as necessary.
}

void step8()
{
        // Choose any nontree edge(v1,w1).
        // Locate the corresponding cycle by following parent pointers from v1 and w1.
        // Compute the cost on each side of this cycle by scanning the tree edges incidient on either side of the cycle and summing their associated costs.
                // If (v,w) is a tree edge with v on the cycle and w not on the cycle
                        // cost associated with (v,w) = v.parent_of(w)       ?
                        //                              descendant cost of w :
                        //                              cost of all vertices minus the descendant cost of v
        // Let side of cycle with greater cost be inside
}

void step9scan()
{
        // Scan the tree edges inside the (y, wi) cycle, alternately scanning an edge in one cycle and an edge in the other cycle.
        // Stop scanning when all edges inside one of the cycles have been scanned.
}

void step9()
{
        // Let (vi, wi) be the nontree dege whose cycle is the current candidate to complete the separator.
        while( false /* the cost inside the cycle exceeds 2/3 */ ){ // find a better cycle

                // Locate the triangle (vi, y, wi) which has (vi, wi) as a boundary edge and lies inside the (vi, wi) cycle.
                if ( true /* (vi, y) is a tree edge || (y, wi) is a tree edge */ ){
                        // let (vi+1, wi+1) = the nontree edge among (vi, y) and (y, wi).  
                        // Compute the cost inside the (vi+1, wi+1) cycle from the cost inside the (vi, wi) cycle and the cost of vi, y, and wi.
                } else {
                        // Determine the tree path from y to the (vi, wi) cycle by following parent pointers from y.
                        // Let z be the vertex on the (vi, wi) cycle reached during this search.
                        // Compute the total cost of all vertices except z on this tree path.
                        step9scan();
                        // Compute the cost inside this cycle by summing the associated costs of all scanned edges.
                        // Use this cost, the cost inside the (vi, wi) cycle, and the cost on the tree path from y to zy to compute the cost inside the other cycle.
                        // Let (vi+1, w+1) be the edge among (vi, y) and (y, wi) whose cycle has more cost inside it.
                }

        }
}

void step10()
{
        // Use the cycle found in step 9 and the levels found in Step 4 to construct a satisfactory vertex partition as described in the proof of Lemma 3.
        // Extend this partition from the connected component chosen in Step 2 to the entire graph as desribed in the proof Theorem 4.
}

void chrobak_payne()
{ 
        typedef adjacency_list<vecS, vecS, undirectedS, property<vertex_index_t, uint>> graph; 

        //Define the storage type for the planar embedding
        typedef vector<vector<graph_traits<graph>::edge_descriptor>> embedding_storage_t;
        typedef iterator_property_map<embedding_storage_t::iterator, property_map<graph, vertex_index_t>::type> embedding_t; 

        // Create the graph - a maximal planar graph on 7 vertices. The functions planar_canonical_ordering and chrobak_payne_straight_line_drawing both require a maximal planar graph. If you start with a graph that
        // isn't maximal planar (or you're not sure), you can use the functions make_connected, make_biconnected_planar, and make_maximal planar in sequence to add a set of edges to any undirected planar graph to make
        // it maximal planar.

        graph g(7);
        add_edge(0,1,g); add_edge(1,2,g); add_edge(2,3,g); add_edge(3,0,g); add_edge(3,4,g); add_edge(4,5,g); add_edge(5,6,g); add_edge(6,3,g);
        add_edge(0,4,g); add_edge(1,3,g); add_edge(3,5,g); add_edge(2,6,g); add_edge(1,4,g); add_edge(1,5,g); add_edge(1,6,g); 

        // Create the planar embedding
        embedding_storage_t embedding_storage(num_vertices(g));
        embedding_t embedding(embedding_storage.begin(), get(vertex_index,g));

        boyer_myrvold_planarity_test(boyer_myrvold_params::graph = g, boyer_myrvold_params::embedding = embedding); 

        // Find a canonical ordering
        vector<graph_traits<graph>::vertex_descriptor> ordering;
        planar_canonical_ordering(g, embedding, back_inserter(ordering));

        //Set up a property map to hold the mapping from vertices to coord_t's
        typedef vector<coord_t>                                                                                             straight_line_drawing_storage_t;
        typedef iterator_property_map<straight_line_drawing_storage_t::iterator, property_map<graph, vertex_index_t>::type> straight_line_drawing_t;

        straight_line_drawing_storage_t straight_line_drawing_storage(num_vertices(g));
        straight_line_drawing_t         straight_line_drawing(straight_line_drawing_storage.begin(), get(vertex_index,g)); 

        chrobak_payne_straight_line_drawing(g, embedding, ordering.begin(), ordering.end(), straight_line_drawing); // Compute the straight line drawing

        cout << "The straight line drawing is: " << endl;
        graph_traits<graph>::vertex_iterator vi, vi_end;
        for( tie(vi,vi_end) = vertices(g); vi != vi_end; ++vi ){
                coord_t coord(get(straight_line_drawing,*vi));
                cout << *vi << " -> (" << coord.x << ", " << coord.y << ")\n";
        }

        cout << is_straight_line_drawing(g, straight_line_drawing) ? "Is a plane drawing.\n" : cout << "Is not a plane drawing.\n";
}

void draw()
{
}

Partition lipton_tarjan(Graph)
{
        Partition p;
        step1();
        if( step2() ) return p;
        step3();
        step4();
        step5();
        step6();
        step7();
        step8();
        step9();
        step10();
        chrobak_payne();
        draw();
        return p;
}
