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
                auto parent = source(e, g);
                auto child  = target(e, g);
                bfs_vertex_data[child].parent = parent;
                bfs_vertex_data[child].level  = bfs_vertex_data[parent].level+1;
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
                cout << "   doit edge " << e << " with " << v << " as source\n";
                cout << "   looking up table[" << w << "] = " << (*table)[w] << '\n';
                if ( !(*table)[w] ){
                        (*table)[w] = true;
                        cout << "     !!!! adding edge to x\n";
                        cout << "w: " << w << ", x: " << x << '\n';
                        assert(x != w);
                        edges_to_add.push_back(make_pair(x, w));
                }
                cout << "   ------------ removing edge doit" << e << '\n';
                edges_to_delete.push_back(make_pair(v, w));
        }

        void finish()
        {
                cout << "finishing lambda\n";
                cout << "edges_to_add size: " << edges_to_add.size() << '\n';
                for( auto& p : edges_to_add    ){
                        cout << "adding edge " << p.first << ", " << p.second << '\n';
                        assert(p.first != p.second);
                        add_edge(p.first, p.second, *g);
                }
                cout << "removing_edges size: " << edges_to_delete.size() << '\n';
                for( auto& p : edges_to_delete ){
                        cout << "removing edge " << p.first << ", " << p.second << '\n';
                        remove_edge(p.first, p.second, *g);
                }
        }
};

void scan_nonsubtree_edges(VertexDescriptor v, Graph const& g, Embedding& em, Lambda lambda)
{
        if( bfs_vertex_data[v].level > lambda.l0 ){
                cout << "skipping vertex " << v << " because level " << bfs_vertex_data[v].level << " > l0\n";
                return;
        }

        cout << "scanning vertex " << v << '\n';
        for( auto e : em[v]       ){
                if( !is_tree_edge(e, g) ) lambda.doit(v, e); 
                else {
                        // if child is skippable we still process it
                        auto src = source(e, g);
                        auto tar = target(e, g);
                        if( src != v ) swap(src, tar);
                        assert(src == v);
                        cout << "   found tree edge " << e << '\n';
                        cout << "   target level: " << bfs_vertex_data[tar].level << '\n';
                        if( bfs_vertex_data[tar].level > lambda.l0 ){
                                cout << "    processing anyway because not part of subtree\n";
                                lambda.doit(v, e);
                        }
                }
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

void theorem4()
{
        cout << "theorem4\n";
        exit(0);
}

void lemma2()
{
}

void lemma3()
{
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

Partition lipton_tarjan(Graph const& gin)
{ 
        Graph g = gin;
        uint n = num_vertices(g); 

        //
        // Step 1 - find a planar embedding of g
        //
        EmbeddingStorage storage(n);
        Embedding        em(storage.begin());
        bool planar = boyer_myrvold_planarity_test(g, em);
        assert(planar); 

        //
        // Step 2 - find connected components of g
        //
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
        if( !too_big ) theorem4();

        //
        // Step 3 - create BFS tree of most costly component. Compute the level of each vertex and the # of of verts L(l) in each level l.
        // 
        bfs_visitor_buildtree vis;
        bfs_vertex_data[0].parent = 0;
        bfs_vertex_data[0].level  = 0;
        breadth_first_search(g, vertex(0, g), visitor(vis)); 
        for( auto& c : children ) sort(c.second.begin(), c.second.end());

        vector<uint> L(num_levels+1); 
        for( auto& d : bfs_vertex_data ) ++L[d.second.level];

        //
        // Step 4
        //
        uint k  = L[0];
        int l1 = 0;
        while( k <= n/2 ) k += L[++l1];
        
        //
        // Step 5
        // 
        float sq  = 2 * sqrt(k);
        float snk = 2 * sqrt(n - k);

        int l0 = l1;     for( ;; ){ if( L.at(l0) + 2*(l1 - l0)     <= sq  ) break; --l0; } 
        int l2 = l1 + 1; for( ;; ){ if( L.at(l2) + 2*(l2 - l1 - 1) <= snk ) break; ++l2; }

        //
        // Step 6
        //
        cout << "\n------- Step 6 --------\n\n";
        vector<VertexDescriptor> verts_to_be_removed;
        for( auto paii = vertices(g); paii.first != paii.second; ++paii.first ){
                auto v = *paii.first;
                if( bfs_vertex_data[v].level >= l2 ){
                        clear_vertex(v, g);
                        cout << "going to delete vertex " << v << '\n';
                        verts_to_be_removed.push_back(v);
                        bfs_vertex_data.erase(bfs_vertex_data.find(v)); // delay actually deleting so descriptors aren't invalidated
                        --n;
                }
        }
        auto x = add_vertex(g); ++n; // represents all verts on level 0 through l0.  
        cout << "n = " << n << '\n';
        map<VertexDescriptor, bool> table;
        
        for( auto pai = vertices(g); pai.first != pai.second; ++pai.first ){
                auto v = *pai.first;
                cout << "level of " << v << ": " << bfs_vertex_data[v].level << ' ';
                table[v] = bfs_vertex_data[v].level <= l0;
                if( table[v] ) cout << "table[" << v << "] = TRUE\n";
                else cout << "table[" << v << "] = FALSE\n";
        }

        // Scan the edges incident to this tree clockwise around the tree.  When scanning an edge(v,w) with v in the tree...
        Lambda lambda(&table, &g, x, l0); 
        cout << "scanning nonsubtree edges...\n";
        scan_nonsubtree_edges(0, g, em, lambda);
        lambda.finish();

        uint offset = 0;
        for( auto& v : verts_to_be_removed ){
                auto vv = v - offset;
                cout << "deleting vertex " << vv << '\n';
                assert(degree(vv, g) == 0);
                clear_vertex(vv, g);
                remove_vertex(vv, g);
                ++offset;
        } 

        lemma2(); 

        //
        // Step 7
        // 
        if( degree(x, g) == 0 ){
                cout << "!!!! vertex x (" << x << ") has no inbound vertices!  Deleting!!!!!\n";
                remove_vertex(x, g);
                x = 0;
        }
        cout << "\n------ Step 7 ------\n\n";
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

        //
        // Step 8
        //
        cout << "\n------ Step 8 ------\n\n";

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





        //
        // Step 9
        //
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
                                // Scan the tree edges inside the (y, wi) cycle, alternately scanning an edge in one cycle and an edge in the other cycle.
                                // Stop scanning when all edges inside one of the cycles have been scanned.
                        // Compute the cost inside this cycle by summing the associated costs of all scanned edges.
                        // Use this cost, the cost inside the (vi, wi) cycle, and the cost on the tree path from y to zy to compute the cost inside the other cycle.
                        // Let (vi+1, w+1) be the edge among (vi, y) and (y, wi) whose cycle has more cost inside it.
                }

        }


        //
        // Step 10
        //
        // Use the cycle found in step 9 and the levels found in Step 4 to construct a satisfactory vertex partition as described in the proof of Lemma 3.
        lemma3();
        // Extend this partition from the connected component chosen in Step 2 to the entire graph as desribed in the proof Theorem 4.
        theorem4();

        Partition p;
        return p;
}
