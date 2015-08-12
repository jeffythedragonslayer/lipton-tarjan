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
#include <boost/graph/make_biconnected_planar.hpp>
#include <boost/graph/make_maximal_planar.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/pending/indirect_cmp.hpp>
#include <boost/range/irange.hpp> 
using namespace std;
using namespace boost; 

int levi_civita(uint i, uint j, uint k)
{
        if( i == j || j == k || k == i ) return 0; 
        if( i == 1 && j == 2 && k == 3 ) return 1;
        if( i == 2 && j == 3 && k == 1 ) return 1;
        if( i == 3 && j == 1 && k == 2 ) return 1;
        return -1;
}

struct BFSVertexData
{
        VertexDescriptor parent;
        int              level;
}; 

struct BFSVertexData2
{
        VertexDescriptor parent;
        uint             cost;
};

map<VertexDescriptor, vector<VertexDescriptor>> children, children2;

map<VertexDescriptor, BFSVertexData>  bfs_vertex_data; // why can't this be inside bfs_visitor_buildtree
map<VertexDescriptor, BFSVertexData2> bfs_vertex_data2;
int num_levels = 1;

struct bfs_visitor_buildtree : public default_bfs_visitor
{ 
        template<typename Edge, typename Graph> void tree_edge(Edge e, Graph const& g)
        {
                cout << "   tree edge " << e << '\n';
                auto parent = source(e, g);
                auto child  = target(e, g);
                bfs_vertex_data[child].parent = parent;
                bfs_vertex_data[child].level  = bfs_vertex_data[parent].level+1;
                cout << "level of " << child << " = " << bfs_vertex_data[child].level << '\n';
                num_levels = max(num_levels, bfs_vertex_data[child].level+1);
                children[parent].push_back(child);
        }

};

bool is_tree_edge(EdgeDescriptor e, Graph const& g)
{
        auto src = source(e, g);
        auto tar = target(e, g);

        return bfs_vertex_data[src].parent == tar || 
               bfs_vertex_data[tar].parent == src;
}

bool is_tree_edge2(EdgeDescriptor e, Graph const& g)
{
        auto src = source(e, g);
        auto tar = target(e, g);

        return bfs_vertex_data2[src].parent == tar || 
               bfs_vertex_data2[tar].parent == src;
}

bool on_cycle(EdgeDescriptor e, vector<VertexDescriptor> const& cycle_verts, Graph const& g)
{
        auto src = source(e, g);
        auto tar = target(e, g);
        return find(cycle_verts.begin(), cycle_verts.end(), src) != cycle_verts.end() &&
               find(cycle_verts.begin(), cycle_verts.end(), tar) != cycle_verts.end();
}

bool edge_inside(EdgeDescriptor e, VertexDescriptor v, vector<VertexDescriptor> const& cycle_verts, Graph const& g, Embedding& em)
{
        cout << "        testing if edge " << e << " is inside the cycle\n";
        auto it     = find(cycle_verts.begin(), cycle_verts.end(), v);
        auto before = it == cycle_verts.begin() ?
                      cycle_verts.end()-1       :
                      it-1;
        auto after  = it+1 == cycle_verts.end() ?
                      cycle_verts.begin()       :
                      it+1;
        cout << "               v: " << v << '\n';
        cout << "               before: " << *before << '\n';
        cout << "               after: " << *after << '\n';

        auto other = (source(e, g) == v) ?
                     target(e, g)        :
                     source(e, g);

        cout << "               other: " << other << '\n';

        vector<uint> perm;
        for( auto& tar_it : em[*it] ){
                assert(source(tar_it, g) == v);
                if( target(tar_it, g) == other   ) perm.push_back(1);
                if( target(tar_it, g) == *before ) perm.push_back(2);
                if( target(tar_it, g) == *after  ) perm.push_back(3);
        }
        assert(perm.size() == 3);
        cout << "               levi civita symbol: " << perm[0] << ' ' << perm[1] << ' ' << perm[2] << '\n';
        return levi_civita(perm[0], perm[1], perm[2]) == 1;
}

vector<pair<VertexDescriptor, VertexDescriptor>> edges_to_delete;
vector<pair<VertexDescriptor, VertexDescriptor>> edges_to_add;

struct Lambda
{
        map<VertexDescriptor, bool>* table;
        Graph*                       g;
        VertexDescriptor             x;
        int                          l0;


        Lambda(map<VertexDescriptor, bool>* table, Graph* g, VertexDescriptor x, int l0) : table(table), g(g), x(x), l0(l0) {}

        void doit(VertexDescriptor V, EdgeDescriptor e)
        {
                auto v = source(e, *g);
                auto w = target(e, *g);
                if( V != v ) swap(v, w);
                assert(V == v);
                if ( !(*table)[w] ){
                        (*table)[w] = true;
                        assert(x != w);
                        edges_to_add.push_back(make_pair(x, w));
                }
                edges_to_delete.push_back(make_pair(v, w));
        }

        void finish()
        {
                for( auto& p : edges_to_add    ){
                        assert(p.first != p.second);
                        add_edge(p.first, p.second, *g);
                }
                for( auto& p : edges_to_delete ){
                        cout << "shrinking edge " << p.first << ", " << p.second << '\n';
                        remove_edge(p.first, p.second, *g);
                }
        }
};

void scan_nonsubtree_edges(VertexDescriptor v, Graph const& g, Embedding& em, Lambda lambda)
{
        if( bfs_vertex_data[v].level > lambda.l0 ) return;

        for( auto e : em[v]       ){
                if( !is_tree_edge(e, g) ){
                        lambda.doit(v, e); 
                        continue;
                } 
                auto src = source(e, g);
                auto tar = target(e, g);
                if( src != v ) swap(src, tar);
                assert(src == v);
                if( bfs_vertex_data[tar].level > lambda.l0 ) lambda.doit(v, e);
        }
        for( auto c : children[v] ) scan_nonsubtree_edges(c, g, em, lambda);
}

set<EdgeDescriptor> tree_edges;

struct bfs_visitor_shrinktree : public default_bfs_visitor
{ 
        template<typename Edge, typename Graph> void tree_edge(Edge e, Graph const& g)
        {
                cout << "shrinktree edge " << e << '\n';
                auto parent = source(e, g);
                auto child  = target(e, g);
                bfs_vertex_data2[child].parent = parent;
                tree_edges.insert(e);

                auto v = child; 
                while( true ){ // TODO make this faster
                        ++bfs_vertex_data2[v].cost;
                        if( v == bfs_vertex_data2[v].parent ) break;
                        v = bfs_vertex_data2[v].parent;
                } 
                children2[parent].push_back(child);
        }

};

uint theorem4(uint partition, Graph const& g)
{
        /*
        Assume G is connected.
        Partition the vertices into levels according to their distance from some vertex v.
        L[l] = # of vertices on level l
        If r is the maximum distance of any vertex from v, define additional levels -1 and r+1 containing no vertices
        l1 = the level such that the sum of costs in levels 0 thru l1-1 < 1/2, but the sum of costs in levels 0 thru l1 is >= 1/2
        (If no such l1 exists, the total cost of all vertices < 1/2, and B = C = {} and return true)
        k = # of vertices on levels 0 thru l1.
        Find a level l0 such that l0 <= l1 and |L[l0]| + 2(l1-l0) <= 2sqrt(k)
        Find a level l2 such that l1+1 <= l2 and |L[l2] + 2(l2-l1-1) <= 2sqrt(n-k)
        If 2 such levels exist, then by Lemma 3 the vertices of G can be partitioned into three sets A, B, C such that no edge joins a vertex in A with a vertex in B,
        neither A or C has cost > 2/3, and C contains no more than 2(sqrt(k) + sqrt(n-k)) vertices.
        But 2(sqrt(k) + sqrt(n-k) <= 2(sqrt(n/2) + sqrt(n/2)) = 2sqrt(2)sqrt(n)
        Thus the theorem holds if suitable levels l0 and l2 exist
                Suppose a suitable level l0 does not exist.  Then, for i <= l1, L[i] >= 2sqrt(k) - 2(l1-i)
                Since L[0] = 1, this means 1 >= 2sqrt(k) - 2l1 and l1 + 1/2 >= sqrt(k).  Thus l1 = floor(l1 + 1/2) > 
                Contradiction

        Now suppose G is not connected
        Let G1, G2, ... , Gk be the connected components of G, with vertex sets V1, V2, ... , Vk respectively.
        If no connected component has total vertex cost > 1/3, let i be the minimum index such that the total cost of V1 U V2 U ... U Vi > 1/3
        A = V1 U V2 U ... U Vi
        B = Vi+1 U Vi+2 U ... U Vk
        C = {}
        Since i is minimum and the cost of Vi <= 1/3, the cost of A <= 2/3. return true;
        If some connected component (say Gi) has total vertex cost between 1/3 and 2/3,
        A = Vi
        B = V1 U ... U Vi-1 U Vi+1 U ... U Vk
        C = {}
        return true

        Finally, if some connected component (say Gi) has total vertex cost exceeding 2/3,
        apply the above argument to Gi
        Let A*, B*, C* be the resulting partition.
        A = set among A* and B* with greater cost
        C = C*
        B = remanining vertices of G
        Then A and B have cost <= 2/3
        return true;

        In all cases the separator C is either empty or contained in only one connected component of G
        */
        return partition;
}

void lemma2()
{
        /*
        Embed G in the plane
        Make each face a triangle by adding a suitable # of additional edges
        Any nontree edge (including each of the added edges) forms a simple cycle with some of the tree edges
        This cycle is of length at most 2r + 1 if it contains the root of the tree, at most 2r-1 otherwise
        The cycle divides the plane (and the graph) into two parts, the inside and outside
        We claim that at least one such cycle separates the graph so that neither the inside nor the outside contains vertices whose total cost > 2/3
                Let (x, z) be the nontree edge whose cycle minimizes the maximum cost either inside or outside the cycle.
                Break ties by choosing the nontree edge whose cycle has the smallest # of faces on the same side as the maximum cost.
                If ties remain, choose arbitrarily
                Suppose wihtout loss of generality that the graph is embedded so that the cost inside the (x, z) cycle is at least as great as the cost outside the cycle.
                If the vertices inside the cycle have total cost <= 2/3, return true
                Suppose that the vertices inside the cycle have total cost > 2/3.  Contradiction.
        */
}

uint lemma3(vector<VertexDescriptor> const& cycle_verts, int* l, Graph const& g)
{
        if( l[1] > l[2] ){
                cout << "A = all verts on levels 0    thru l1-1";
                cout << "B = all verts on levels l1+1 thru r";
                cout << "C = all verts on llevel l1";
        }
                
        if( l[1] < l[2] ){
                cout << "don't know\n";
                vector<VertexDescriptor> zero_one, middle_part, one_two;
                for( uint i = 0; i < bfs_vertex_data.size(); ++i ){
                        if( bfs_vertex_data[i].level <= l[1] ){ 
                                cout << "first part: " << i << '\n';
                                zero_one.push_back(i);
                                continue;
                        }
                        if( bfs_vertex_data[i].level >= l[1]+1 && 
                            bfs_vertex_data[i].level <= l[2]-1 ){
                                cout << "middle part: " << i << '\n';
                                middle_part.push_back(i);
                                continue;
                        }
                        if( bfs_vertex_data[i].level >= l[2] ){
                                cout << "last part: " << i << '\n';
                                one_two.push_back(i);
                                continue;
                        }
                        cout << "level: " << bfs_vertex_data[i].level << '\n';
                        assert(0);
                }


                //delete verts in levels l1 and l2
                        /*this separates remaining vertices into 3 parts: (all of which may be empty)
                                verts on levels 0 thru l1-1
                                verts on level l1+1 thru l2-1
                                verts on levels l2+1 and above
                        the only part which can have cost > 2/3 is the middle part*/
                        if( middle_part.size() <= 2*num_vertices(g)/3 ){
                                cout << "A = most costly part of the 3\n";
                                cout << "B = remaining 2 parts\n";
                                cout << "C = "; for( auto& v : one_two ) cout << v << ' '; cout << '\n';
                        } else {
                                /*delete all verts on level l2 and above
                                shrink all verts on levels l1 and belowe to a single vertex of cost zero
                                The new graph has a spanning tree radius of l2 - l1 -1 whose root corresponds to vertices on levels l1 and below in the original graph
                                Apply Lemma 2 to the new graph, A* B* C**/
                                cout << "A = set among A* and B* with greater cost\n";
                                cout << "C = verts on levels l1 and l2 in the original graph plus verts in C* minus the root\n";
                                cout << "B = remaining verts\n";
                                /* By Lemma 2, A has total cost <= 2/3
                                But A U C* has total cost >= 1/3, so B also has total cost <= 2/3
                                Futhermore, C contains no more than L[l1] + L[l2] + 2(l2 - l1 - 1)*/
                        }
        }
        return 0;
}

void print_canonical_ordering(Graph const& g, vector<VertexDescriptor> const& ordering, Embedding const& em)
{ 
        for( auto& v : ordering ){
                cout << "vertex " << v << "\n";
                for( auto& e : em[v] ){
                        cout << " has incident edge " << e << '\n';
                }
        } 
}

void print_graph(Graph const& g)
{ 
        cout << "\n*********************************************************\n";
        auto pai = vertices(g);
        while( pai.first != pai.second ){
                cout << "vertex " << *pai.first << '\n';
                auto pai2 = out_edges(*pai.first, g);
                while( pai2.first != pai2.second ){
                        cout << " has incident edge " << *pai2.first << '\n';
                        ++pai2.first;
                }
                ++pai.first;
        } 
        cout << "*********************************************************\n\n";
}

void print_bfs_tree(vector<uint> const& L)
{
        for( uint i = 0; i < bfs_vertex_data.size(); ++i ) cout << "parent of " << i << ": " << bfs_vertex_data[i].parent << ", level: " << bfs_vertex_data[i].level << '\n'; 
        for( uint i = 0; i < L.size();               ++i ) cout << "L[" << i << "] = " << L[i] << '\n';
}

void makemaxplanar(Graph& g)
{ 
        auto e_index = get(edge_index, g);
        EdgesSizeType num_edges = 0;
        EdgeIterator ei, ei_end;
        for( tie(ei, ei_end) = edges(g); ei != ei_end; ++ei ) put(e_index, *ei, num_edges++);

        vector<vector<EdgeDescriptor>> em(num_vertices(g));
        boyer_myrvold_planarity_test(g, &em[0]);

        make_biconnected_planar(g, &em[0]);

        num_edges = 0;
        for( tie(ei, ei_end) = edges(g); ei != ei_end; ++ei ) put(e_index, *ei, num_edges++); 
        boyer_myrvold_planarity_test(g, &em[0]);

        make_maximal_planar(g, &em[0]);

        num_edges = 0;
        for( tie(ei, ei_end) = edges(g); ei != ei_end; ++ei ) put(e_index, *ei, num_edges++); 
        bool planar = boyer_myrvold_planarity_test(g, &em[0]);
        assert(planar); 
} 

uint edge_cost(EdgeDescriptor e, Graph const& g)
{
        assert(is_tree_edge2(e, g));
        auto v = source(e, g); // assert on the cycle
        auto w = target(e, g); // assert not on the cycle

        return bfs_vertex_data2[w].parent == w ?
               bfs_vertex_data2[w].cost        :
               num_vertices(g) - bfs_vertex_data2[v].cost;
}

Partitionh lipton_tarjan(Graph const& gin)
{ 
        Graph g = gin;
        uint n = num_vertices(g); 

        cout << "\n---------------------------- Step 1 --------------------------\n"; 
        EmbeddingStorage storage(n);
        Embedding        em(storage.begin());
        bool planar = boyer_myrvold_planarity_test(g, em);
        assert(planar); 

        print_graph(g);

        cout << "\n---------------------------- Step 2 --------------------------\n"; 
        vector<uint> vertid_to_component(n);
        uint components = connected_components(g, &vertid_to_component[0]);
        assert(components == 1);

        vector<uint> verts_per_comp(components, 0);
        for( uint i = 0; i < n;          ++i ) ++verts_per_comp[vertid_to_component[i]]; 
        bool too_big = false;
        for( uint i = 0; i < components; ++i ) if( 3*verts_per_comp[i] > 2*n ){
                too_big = true;
                break;
        }
        if( !too_big ){
                theorem4(0, g);
                Partitionh p;
                return p;
        }

        cout << "\n---------------------------- Step 3 --------------------------\n"; 
        bfs_visitor_buildtree vis;
        bfs_vertex_data[0].parent = 0;
        bfs_vertex_data[0].level  = 0;
        breadth_first_search(g, vertex(0, g), visitor(vis)); 
        for( auto& c : children ) sort(c.second.begin(), c.second.end());

        vector<uint> L(num_levels+1); 
        for( auto& d : bfs_vertex_data ) ++L[d.second.level];

        cout << "\n---------------------------- Step 4 --------------------------\n"; 
        uint k  = L[0];
        int l[3];
        l[1] = 0;
        while( k <= n/2 ) k += L[++l[1]];
        cout << "l1: " << l[1] << '\n';
        cout << "k: " << k << '\n';
        
        cout << "\n---------------------------- Step 5 --------------------------\n"; 
        float sq  = 2 * sqrt(k);
        float snk = 2 * sqrt(n - k);

        l[0] = l[1];     for( ;; ){ if( L.at(l[0]) + 2*(l[1] - l[0])     <= sq  ) break; --l[0]; } 
        l[2] = l[1] + 1; for( ;; ){ if( L.at(l[2]) + 2*(l[2] - l[1] - 1) <= snk ) break; ++l[2]; }
        cout << "l0: " << l[0] << '\n';
        cout << "l2: " << l[2] << '\n';

        print_graph(g);

        cout << "\n---------------------------- Step 6 --------------------------\n"; 
        vector<VertexDescriptor> verts_to_be_removed;
        for( auto paii = vertices(g); paii.first != paii.second; ++paii.first ){
                auto v = *paii.first;
                if( bfs_vertex_data[v].level <= l[0] ){
                        cout << "going to shrink vertex " << v << '\n';
                        cout << "   level[v]: " << bfs_vertex_data[v].level << '\n';
                        cout << "   cutoff: " << l[0] << '\n';
                        verts_to_be_removed.push_back(v);
                        --n;
                }
                if( bfs_vertex_data[v].level >= l[2] ){
                        clear_vertex(v, g);
                        verts_to_be_removed.push_back(v);
                        bfs_vertex_data.erase(bfs_vertex_data.find(v));
                        --n;
                }
        }
        auto x = add_vertex(g); ++n; // represents all verts on level 0 thru l0.  
        map<VertexDescriptor, bool> table;
        
        for( auto pai = vertices(g); pai.first != pai.second; ++pai.first ){
                auto v = *pai.first;
                table[v] = bfs_vertex_data[v].level <= l[0];
        }

        // Scan the edges incident to this tree clockwise around the tree.  When scanning an edge(v,w) with v in the tree...
        Lambda lambda(&table, &g, x, l[0]); 
        scan_nonsubtree_edges(0, g, em, lambda);
        lambda.finish();

        cout << "after lambda finishes\n";
        print_graph(g);

        uint offset = 0;
        for( auto& v : verts_to_be_removed ){
                auto vv = v - offset;
                cout << "removing vertex " << v << '\n';
                clear_vertex(vv, g);
                remove_vertex(vv, g);
                ++offset;
                --x;
        } 

        lemma2(); 

        print_graph(g);

        cout << "\n---------------------------- Step 7 --------------------------\n"; 
        //if( degree(x, g) == 0 ){
                //cout << "!!!! vertex x (" << x << ") has no inbound vertices!  Deleting!!!!!\n";
                //remove_vertex(x, g);
                //x = 0;
        //}
        n = num_vertices(g);
        cout << "# verts: " << num_vertices(g) << '\n';
        cout << "# edges: " << num_edges   (g) << '\n';
        cout << "n = " << n << '\n';
        cout << "x = " << x << '\n';

        bfs_visitor_shrinktree vis2;
        bfs_vertex_data2[x].parent = x;
        breadth_first_search(g, x, visitor(vis2));
        for( auto& c : children2 ) sort(c.second.begin(), c.second.end()); 

        makemaxplanar(g);

        cout << "make maximal planar - should have " << 3*n - 6 << " edges\n";
        cout << "# verts: " << num_vertices(g) << '\n';
        cout << "# edges: " << num_edges   (g) << '\n'; 
        n = num_vertices(g);
        uint e = num_edges(g); 
        assert(e == 3*n - 6); 

        cout << "\n---------------------------- Step 8 --------------------------\n"; 

        print_graph(g); 

        EdgeIterator ei, ei_end;
        for( tie(ei, ei_end) = edges(g); ei != ei_end; ++ei ) if( !is_tree_edge2(*ei, g) ) break;
        assert(!is_tree_edge2(*ei, g));
        EdgeDescriptor chosen_edge = *ei;
        cout << "arbitrarily choosing nontree edge: " << chosen_edge << '\n';
        
        auto v1 = source(chosen_edge, g);
        auto w1 = target(chosen_edge, g);
        vector<VertexDescriptor> parents_v, parents_w;

        auto p_v = v1; do { p_v = bfs_vertex_data2[p_v].parent; parents_v.push_back(p_v); } while( p_v );
        auto p_w = w1; do {p_w = bfs_vertex_data2[p_w].parent; parents_w.push_back(p_w); } while( p_w );

        uint i, j;
        for( i = 0; i < parents_v.size(); ++i ) for( j = 0; j < parents_w.size(); ++j ) if( parents_v[i] == parents_w[j] ) goto done;
done:
        assert(parents_v[i] == parents_w[j]);
        auto ancestor = parents_v[i];
        cout << "common ancestor: " << ancestor << '\n';

        vector<VertexDescriptor> cycle_verts, tmp;
        VertexDescriptor v;
        v = v1; while( v != ancestor ){ cycle_verts.push_back(v); v = bfs_vertex_data2[v].parent; } 
        cycle_verts.push_back(ancestor);
        v = w1; while( v != ancestor ){ tmp.push_back(v); v = bfs_vertex_data2[v].parent; } 
        reverse(tmp.begin(), tmp.end());
        cycle_verts.insert(cycle_verts.end(), tmp.begin(), tmp.end());

        cout << "cycle verts: ";
        for( auto& v : cycle_verts ) cout << v << ' ';
        cout << '\n';

        EmbeddingStorage storage2(num_vertices(g));
        Embedding        em2(storage2.begin());
        planar = boyer_myrvold_planarity_test(g, em2);
        assert(planar); 

        uint cost_inside  = 0;
        uint cost_outside = 0;
        bool cost_swapped = false;

        for( auto& v : cycle_verts ){
                cout << "   scanning cycle vert " << v << '\n';
                auto pai = out_edges(v, g);
                while( pai.first != pai.second ){
                        if( is_tree_edge2(*pai.first, g) && !on_cycle(*pai.first, cycle_verts, g) ){
                                uint cost = edge_cost(*pai.first, g);
                                cout << "      scanning incident tree edge " << *pai.first << "   cost: " << cost << '\n';
                                bool inside = edge_inside(*pai.first, v, cycle_verts, g, em2);
                                inside ? cost_inside : cost_outside += cost;
                                cout << (inside ? "inside\n" : "outside\n");
                        }
                        ++pai.first;
                }
        }

        if( cost_outside > cost_inside ){
                swap(cost_outside, cost_inside);
                cost_swapped = true;
                cout << "cost swapped\n";
        }
        cout << "total inside cost: " << cost_inside << '\n';

        cout << "\n---------------------------- Step 9 --------------------------\n"; 
        auto vi = source(chosen_edge, g);
        auto wi = target(chosen_edge, g);
        assert(!is_tree_edge2(chosen_edge, g));
        EdgeDescriptor next_edge;
        while( cost_inside > num_vertices(g)*2./3 ){
                cout << "looking for a better cycle\n";

                // Locate the triangle (vi, y, wi) which has (vi, wi) as a boundary edge and lies inside the (vi, wi) cycle.
                set<VertexDescriptor> verts_v, verts_w;
                auto pai = out_edges(vi, g);
                while( pai.first != pai.second ){ 
                        auto vv = target(*pai.first, g);
                        verts_v.insert(vv);
                        cout << "vertex " << vi << "has neighbor " << vv << '\n';
                        ++pai.first;
                }
                pai = out_edges(wi, g);
                while( pai.first != pai.second ){ 
                        auto vv = target(*pai.first, g);
                        verts_w.insert(vv);
                        cout << "vertex " << wi << "has neighbor " << vv << '\n';
                        ++pai.first;
                } 

                // there are 2 triangles with edge (vi, wi), one inside and one outside
                EdgeDescriptor viy, ywi;
                if ( is_tree_edge2(viy, g) || is_tree_edge2(ywi, g) ){
                        next_edge = is_tree_edge2(viy, g) ? ywi : viy;
                        assert(!is_tree_edge2(next_edge, g));
                        // Compute the cost inside the (vi+1, wi+1) cycle from the cost inside the (vi, wi) cycle and the cost of vi, y, and wi.
                } else {
                        // Determine the tree path from y to the (vi, wi) cycle by following parent pointers from y.
                        VertexDescriptor z; // the (vi, wi) cycle reached during this search.
                        // Compute the total cost of all vertices except z on this tree path.
                                // Scan the tree edges inside the (y, wi) cycle, alternately scanning an edge in one cycle and an edge in the other cycle.
                                // Stop scanning when all edges inside one of the cycles have been scanned.
                        // Compute the cost inside this cycle by summing the associated costs of all scanned edges.
                        // Use this cost, the cost inside the (vi, wi) cycle, and the cost on the tree path from y to zy to compute the cost inside the other cycle.
                        // Let (vi+1, w+1) be the edge among (vi, y) and (y, wi) whose cycle has more cost inside it.
                }

        }

        cout << "\n---------------------------- Step 10 --------------------------\n"; 
        uint partition = lemma3(cycle_verts, &l[0], g);
        partition = theorem4(partition, g); 
        Partitionh p;
        return p;
}
