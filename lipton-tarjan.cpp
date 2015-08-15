#include "lipton-tarjan.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <csignal>
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/planar_canonical_ordering.hpp>
#include <boost/graph/is_straight_line_drawing.hpp>
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

ostream& operator<<(ostream& o, VertDesc v)
{
        o << vert2uint[v];
        return o;
}

struct BFSVert
{
        BFSVert() : parent(nullptr), level(0), cost(0) {}

        VertDesc parent;
        int      level;
        uint     cost;
};

struct BFSVisitorData
{
        map<VertDesc, set<VertDesc>> children;
        map<VertDesc, BFSVert>       verts;
        int                          num_levels;
        Graph&                       g;

        BFSVisitorData(Graph& g) : g(g), num_levels(0) {}

        bool is_tree_edge(EdgeDesc e)
        { 
                auto src = source(e, g);
                auto tar = target(e, g); 
                return verts[src].parent == tar || verts[tar].parent == src;
        }

        void print_costs()
        {
                for( auto& v : verts ) cout << "cost of vertex " << v.first << " is " << v.second.cost << '\n';
        }
};

struct BFSVisitor : public default_bfs_visitor
{
        BFSVisitorData& data;

        BFSVisitor(BFSVisitorData& data) : data(data) {}

        template<typename Edge, typename Graph> void tree_edge(Edge e, Graph const& g)
        {
                auto parent = source(e, g);
                auto child  = target(e, g);
                cout << "  tree edge " << parent << ", " << child << '\n';
                data.verts[child].parent = parent;
                data.verts[child].level  = data.verts[parent].level + 1;
                data.num_levels = max(data.num_levels, data.verts[child].level + 1);
                if( Graph::null_vertex() != parent ) data.children[parent].insert(child);

                VertDesc v = child;
                do {
                        ++data.verts[v].cost;
                        cout << "   cost of " << v << " is " << ++data.verts[v].cost << '\n';
                        v =  data.verts[v].parent;
                } while( data.verts[v].level );
        } 
};

extern void print_graph(Graph const& g);

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
        Then A and B have cost <= 2/3, g
        return true;

        In all cases the separator C is either empty or contained in only one connected component of G
        */
        return partition;
}

struct ScanVisitor
{
        map<VertDesc, bool>*          t;
        Graph*                        g;
        VertDesc                      x;
        int                           l0;
        set<pair<VertDesc, VertDesc>> edges_to_add, edges_to_delete;

        ScanVisitor(map<VertDesc, bool>* t, Graph* g, VertDesc x, int l0) : t(t), g(g), x(x), l0(l0) {}

        void foundedge(VertDesc V, EdgeDesc e)
        {
                auto v = source(e, *g);
                auto w = target(e, *g);
                if( V != v ) swap(v, w);
                assert(V == v);
                cout << "foundedge " << v << ", " << w << '\n';
                if ( !(*t)[w] ){
                        (*t)[w] = true;
                        assert(x != w); 
                        cout << "going to add " << x << ", " << w << '\n';
                        edges_to_add.insert(make_pair(x, w));
                }
                cout << "going to delete " << v << ", " << w << '\n';
                edges_to_delete.insert(make_pair(v, w)); 
        }

        void finish()
        {
                cout << "finishing\n";
                cout << "edges to add size: " << edges_to_add.size() << '\n';
                for( auto& p : edges_to_add    ){
                        assert(p.first != p.second);
                        cout << "adding " << p.first << ", " << p.second << '\n';
                        add_edge(p.first, p.second, *g);
                }
                cout << "edges to remove size: " << edges_to_delete.size() << '\n';
                for( auto& p : edges_to_delete ){ 
                        cout << "removing " << p.first << ", " << p.second << '\n';
                        remove_edge(p.first, p.second, *g);
                }
        }
};

void scan_nonsubtree_edges(VertDesc v, Graph const& g, Embedding& em, BFSVisitorData& bfs, ScanVisitor vis)
{
        if( bfs.verts[v].level > vis.l0 ) return;
        for( auto e : em[v] ){
                auto src = source(e, g);
                auto tar = target(e, g);
                if( src == tar ) continue; // ?????
                if( !bfs.is_tree_edge(e) ){
                        vis.foundedge(v, e);
                        continue;
                }
                if( src != v ) swap(src, tar);
                assert(src == v); 
                if( bfs.verts[tar].level > vis.l0 ) vis.foundedge(v, e);
        }
        for( auto c : bfs.children[v] ) scan_nonsubtree_edges(c, g, em, bfs, vis);
}

void reset_vertex_indices(Graph& g)
{
        VertIter vi, vend;
        uint i = 0;
        for( tie(vi, vend) = vertices(g); vi != vend; ++vi, ++i ) put(vertex_index, g, *vi, i); 
}

bool is_planar(Graph& g)
{
        reset_vertex_indices(g);
        EmbeddingStorage storage{num_vertices(g)};
        Embedding        em(storage.begin()); 
        return boyer_myrvold_planarity_test(boyer_myrvold_params::graph = g, boyer_myrvold_params::embedding = em);
}

typedef property_map<Graph, edge_index_t>::type EdgeIndex;
EdgeIndex reset_edge_index(Graph& g)
{
        EdgeIndex edgedesc_to_uint; 
        EdgesSizeType num_edges = 0;
        EdgeIter ei, ei_end;
        for( tie(ei, ei_end) = edges(g); ei != ei_end; ++ei ) edgedesc_to_uint[*ei] = num_edges++;
        return edgedesc_to_uint;
} 

void makemaxplanar(Graph& g)
{ 
        auto index = reset_edge_index(g);
        EmbeddingStorage storage(num_vertices(g));
        Embedding        em(storage.begin());
        boyer_myrvold_planarity_test(boyer_myrvold_params::graph = g, boyer_myrvold_params::embedding = em);
        make_biconnected_planar(g, em, index);

        reset_edge_index(g);
        boyer_myrvold_planarity_test(boyer_myrvold_params::graph = g, boyer_myrvold_params::embedding = em);

        make_maximal_planar(g, em);

        reset_edge_index(g);
        bool planar = boyer_myrvold_planarity_test(boyer_myrvold_params::graph = g, boyer_myrvold_params::embedding = em);
        assert(planar);
} 

Partition lipton_tarjan(Graph& g)
{
        cout << "---------------------------- Step 1 --------------------------\n";
        bool planar = is_planar(g);
        assert(planar);
        cout << "planar ok\n";

        cout << "---------------------------- Step 2 --------------------------\n";
        VertDescMap idx; 
        associative_property_map<VertDescMap> vertid_to_component(idx);
        VertIter vi, vj;
        tie(vi, vj) = vertices(g);
        for( uint i = 0; vi != vj; ++vi, ++i ) put(vertid_to_component, *vi, i);
        uint components = connected_components(g, vertid_to_component);
        assert(components == 1); 
        vector<uint> verts_per_comp(components, 0);
        for( tie(vi, vj) = vertices(g); vi != vj; ++vi ) ++verts_per_comp[vertid_to_component[*vi]]; 
        bool too_big = false;
        for( uint i = 0; i < components; ++i ) if( 3*verts_per_comp[i] > 2*num_vertices(g) ){ too_big = true; break; }
        if( !too_big ){ theorem4(0, g); return {};}

        cout << "---------------------------- Step 3 --------------------------\n";
        BFSVisitorData vis_data(g);
        breadth_first_search(g, *vertices(g).first, visitor(BFSVisitor(vis_data)));

        vector<uint> L(vis_data.num_levels + 1, 0);
        for( auto& d : vis_data.verts ) ++L[d.second.level];

        for( tie(vi, vj) = vertices(g); vi != vj; ++vi ) cout << "level of vert " << *vi << ": " << vis_data.verts[*vi].level << '\n';
        for( uint i = 0; i < L.size(); ++i ) cout << "L[" << i << "]: " << L[i] << '\n';

        cout << "---------------------------- Step 4 --------------------------\n";
        uint k = L[0]; 
        int l[3];
        l[1] = 0;
        while( k <= num_vertices(g)/2 ) k += L[++l[1]];
        cout << "k: " << k << '\n';
        cout << "l1:" << l[1] << '\n';

        cout << "---------------------------- Step 5 --------------------------\n";
        float sq  = 2 * sqrt(k); 
        float snk = 2 * sqrt(num_vertices(g) - k); 
        cout << "sq: " << sq << '\n';
        cout << "snk: " << snk << '\n';

        l[0] = l[1]; for( ;; ){ float val = L.at(l[0]) + 2*(l[1] - l[0]); if( val <= sq  ) break; --l[0]; } 
        cout << "l0: " << l[0] << '\n'; 
        l[2] = l[1] + 1; for( ;; ){ float val = L.at(l[2]) + 2*(l[2] - l[1] - 1); if( val <= snk ) break; ++l[2]; } 
        cout << "l2: " << l[2] << '\n';

        cout << "---------------------------- Step 6 --------------------------\n";
        tie(vi, vj) = vertices(g); 
        for( VertIter next = vi; vi != vj; vi = next){
                ++next;
                if( vis_data.verts[*vi].level >= l[2] ){
                        auto i = vert2uint[*vi];
                        uint2vert[i] = Graph::null_vertex();
                        vert2uint[*vi] = -1;
                        clear_vertex(*vi, g);
                        cout << "deleting vertex " << *vi << " of level " << vis_data.verts[*vi].level << '\n';
                        remove_vertex(*vi, g);
                }
        }

        auto x = add_vertex(g); uint2vert[vert2uint[x] = num_vertices(g)] = x; 
        map<VertDesc, bool> t;
        for( tie(vi, vj) = vertices(g); vi != vj; ++vi ){
                t[*vi] = (vis_data.verts[*vi].level <= l[0]);
                cout << "vertex " << *vi << " is " << t[*vi] << " in table\n";
        }

        reset_vertex_indices(g);
        EmbeddingStorage storage{num_vertices(g)};
        Embedding        em(storage.begin());

        planar = boyer_myrvold_planarity_test(boyer_myrvold_params::graph = g, boyer_myrvold_params::embedding = em);
        assert(planar);

        ScanVisitor svis(&t, &g, x, l[0]);
        scan_nonsubtree_edges(*vertices(g).first, g, em, vis_data, svis);
        svis.finish();

        cout << "---------------------------- Step 7 --------------------------\n";
        print_graph(g);
        BFSVisitorData vis_data2(g);
        vis_data2.print_costs();
        breadth_first_search(g, x, visitor(BFSVisitor(vis_data2)));

        vis_data2.print_costs();

        makemaxplanar(g);

        return {};
}
