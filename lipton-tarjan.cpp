#include "lipton-tarjan.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <csignal>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/planar_canonical_ordering.hpp>
#include <boost/graph/is_straight_line_drawing.hpp>
#include <boost/graph/chrobak_payne_drawing.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp> 
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/pending/indirect_cmp.hpp>
#include <boost/range/irange.hpp> 
using namespace std;
using namespace boost; 

bool find_cc(Graph* g) // Step 2
{ 
        uint N = num_vertices(*g);
        vector<uint> vertid_to_comp(N);
        uint num_comps = connected_components(*g, &vertid_to_comp[0]);

        vector<uint> verts_per_comp(num_comps, 0); 
        for( uint i = 0; i < N;         ++i ) ++verts_per_comp[vertid_to_comp[i]]; 
        for( uint i = 0; i < num_comps; ++i ) if( 3*verts_per_comp[i] > 2*N ) return false;

        // construct the paritition as described in the proof of Theorem 4.
        return true;
}

template<typename TimeMap>
struct bfs_time_visitor : public default_bfs_visitor
{
        typedef typename property_traits<TimeMap>::value_type T;

        TimeMap timemap;
        T&      time;

        bfs_time_visitor(TimeMap tmap, T & t) : timemap(tmap), time(t) { }

        template<typename Vertex, typename Graph>
        void discover_vertex(Vertex u, const Graph & g) const
        {
                put(timemap, u, time++);
        }

};

// Find a breadth-first spanning tree of the most costly component.
// Compute the level of each vertex and the # of of verts L(l) in each level l.
void step3(Graph* gg, uint costly_comp)
{ 
        /*uint N = num_vertices(*gg);

        typedef graph_traits<Graph>::vertices_size_type                                                        Size;
        typedef iterator_property_map<vector<Size>::iterator, property_map<Graph, vertex_index_t>::const_type> DTimePM;

        vector<Size> dtime(N);
        DTimePM dtime_pm(dtime.begin(), get(vertex_index, *gg));

        Size time = 0;
        bfs_time_visitor<DTimePM> vis(dtime_pm, time);
        breadth_first_search(*gg, vertex(0, *gg), visitor(vis));

        vector<graph_traits<Graph>::vertices_size_type> discover_order(N);
        integer_range<uint>range(0, N);
        copy(range.begin(), range.end(), discover_order.begin());
        sort(discover_order.begin(), discover_order.end(), indirect_cmp<DTimePM, less<Size>>(dtime_pm));*/
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
        // make_biconnected_planar(g, &embedding[0]);
        // make_maximal_planar(g, &embedding[0]);
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

void planar(Graph* g, Partition* p) // Step 1
{
        p->embedding_storage = new EmbeddingStorage(num_vertices(*g));
        p->embedding         = new Embedding(p->embedding_storage->begin(), get(vertex_index, *g)); 
        
        boyer_myrvold_planarity_test(*g, *p->embedding); 
        assert(num_vertices(*g) >= 3);

        planar_canonical_ordering(*g, *p->embedding, back_inserter(p->ordering)); 
        
}

Partition lipton_tarjan(Graph g)
{
        Partition p;

        planar(&g, &p); 
        if( find_cc(&g) ) return p; 
        step3(&g, p.costly_component); 
        step4(); 
        step5(); 
        step6(); 
        step7(); 
        step8(); 
        step9(); 
        step10();
        return p;
}
