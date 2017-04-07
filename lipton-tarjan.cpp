#include "lipton-tarjan.h"
#include "colors.h"
#include "strutil.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <csignal>
#include <boost/lexical_cast.hpp>
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
#include <boost/graph/copy.hpp>
using namespace std;
using namespace boost; 
#define STLALL(x) (x).begin(), (x).end()

#define HEADER_COL GREEN

Partition empty_partition;

int levi_civita(uint i, uint j, uint k)
{
        if( i == j || j == k || k == i ) return 0; 
        if( i == 1 && j == 2 && k == 3 ) return 1;
        if( i == 2 && j == 3 && k == 1 ) return 1;
        if( i == 3 && j == 1 && k == 2 ) return 1;
        return -1;
} 

bool on_cycle(VertDesc v, vector<VertDesc> const& cycle, Graph const& g)
{
        return find(STLALL(cycle), v) != cycle.end();
}

bool on_cycle(EdgeDesc e, vector<VertDesc> const& cycle, Graph const& g)
{
        auto src = source(e, g);
        auto tar = target(e, g);
        return on_cycle(src, cycle, g) && on_cycle(tar, cycle, g);
}

enum InsideOutOn {INSIDE, OUTSIDE, ON};

InsideOutOn edge_inside_cycle(EdgeDesc e, VertDesc common_vert, vector<VertDesc> const& cycle, Graph const& g, Embedding const& em)
{
        //cout << "cycle: ";
        //for( uint i = 0; i < cycle.size(); ++i ) cout << cycle[i] << ' ';
        //cout << '\n';
        auto src = source(e, g);
        auto tar = target(e, g);
        if( on_cycle(e, cycle, g) ) return ON;
        //cout << "      testing if edge " << src << ", " << tar << " is inside the cycle: ";
        //cout << "      common_vert:    " << common_vert   << '\n';
        auto it = find(STLALL(cycle), common_vert);
        if( it == cycle.end() ){ cout << "      not here at all!\n"; assert(0); }
        assert(*it == common_vert);
        auto before = it   == cycle.begin() ?  cycle.end  ()-1   : it-1;
        auto after  = it+1 == cycle.end  () ?  cycle.begin()     : it+1; 
        auto other  = source(e, g) == common_vert ? target(e, g) : source(e, g); 
        
        //cout << '\n';
        //cout << "      it:     " << *it         << '\n';
        //cout << "      v:      " << common_vert << '\n';
        //cout << "      before: " << *before     << '\n';
        //cout << "      after:  " << *after      << '\n';
        //cout << "      other:  " << other       << '\n';

        vector<uint> perm;
        set<VertDesc> seenbefore;
        for( auto& tar_it : em[*it] ){ // why does this contain duplicates?
                auto src = source(tar_it, g);
                auto tar = target(tar_it, g);
                if( src != common_vert ) swap(src, tar);
                assert(src == common_vert);
                if( seenbefore.find(tar) != seenbefore.end() ) continue;
                seenbefore.insert(tar);

                if(      tar == other   ) perm.push_back(1);
                else if( tar == *before ) perm.push_back(2);
                else if( tar == *after  ) perm.push_back(3);
        } 
        assert(perm.size() == 3);

	return levi_civita(perm[0], perm[1], perm[2]) == 1 ?
	       INSIDE					   :
	       OUTSIDE;
}


struct BFSVert
{
        BFSVert() : parent(Graph::null_vertex()), level(0), descendant_cost(0) {}

        VertDesc parent;
        int      level;
        uint     descendant_cost;
};

struct BFSVisitorData
{
        map<VertDesc, set<VertDesc>> children;
        map<VertDesc, BFSVert>       verts;
        int                          num_levels;
        Graph*                       g;
        VertDesc                     root;

        BFSVisitorData(Graph* g) : g(g), num_levels(0), root(Graph::null_vertex()) {}

        void reset(Graph* g)
        {
                children.clear();
                verts   .clear();
                num_levels = 0;
                this->g = g;
                root = Graph::null_vertex();
        }
        
        bool is_tree_edge(EdgeDesc e) const
        { 
                //cout << "testing is tree edge " << to_string(e, *g) << '\n';
                auto src = source(e, *g);
                auto tar = target(e, *g); 
                auto src_it = verts.find(src);
                auto tar_it = verts.find(tar);
                //cout << "src: " << src << '\n';
                //cout << "tar: " << tar << '\n';
                assert(src_it != verts.end());
                assert(tar_it != verts.end());
                return src_it->second.parent == tar || tar_it->second.parent == src;
        }

        uint edge_cost(EdgeDesc e, vector<VertDesc> const& cycle, Graph const& g) const
        {
                assert(is_tree_edge(e));

                auto v = source(e, g); 
                auto w = target(e, g); 
                auto v_it = verts.find(v); assert(v_it != verts.end());
                auto w_it = verts.find(w); assert(w_it != verts.end());
                if( !on_cycle(v, cycle, g) ) swap(v, w);

                assert( on_cycle(v, cycle, g));
                assert(!on_cycle(w, cycle, g));

                uint total = num_vertices(g);

                assert(w_it->second.parent == v || v_it->second.parent == w);
                return w_it->second.parent == v ? w_it->second.descendant_cost : total - v_it->second.descendant_cost;
        } 

        void print_costs  () const {for( auto& v : verts ) cout << "descendant cost of vertex " << v.first << " is " << v.second.descendant_cost << '\n';}
        void print_parents() const {for( auto& v : verts ) cout << "parent of " << v.first << " is " << v.second.parent << '\n';}
};

struct BFSVisitor : default_bfs_visitor
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
                data.verts[v].descendant_cost = 1;
                //cout << "     vertex/descendant cost: ";
                //cout << v << '/'  << data.verts[v].descendant_cost << "   ";
                while( data.verts[v].level ){
                        v =  data.verts[v].parent;
                        ++data.verts[v].descendant_cost;
                        //cout << v << '/'  << data.verts[v].descendant_cost << "   ";
                } 
                //cout << '\n';
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
        Then A and B have cost <= 2/3, g
        return true;

        In all cases the separator C is either empty or contained in only one connected component of G
        */
        return partition;
}

struct ScanVisitor
{
        map<VertDesc, bool>*          table;
        Graph*                        g;
        VertDesc                      x;
        int                           l0;
        set<pair<VertDesc, VertDesc>> edges_to_add, edges_to_delete;

        ScanVisitor(map<VertDesc, bool>* table, Graph* g, VertDesc x, int l0) : table(table), g(g), x(x), l0(l0) {}

        void foundedge(VertDesc V, EdgeDesc e)
        {
                auto v = source(e, *g);
                auto w = target(e, *g);
                if( V != v ) swap(v, w);
                assert(V == v);
                cout << "foundedge " << v << ", " << w;
                if ( !(*table)[w] ){
                        (*table)[w] = true;
                        assert(x != w); 
                        cout << "   !!!!!!!going to add " << x << ", " << w;
                        edges_to_add.insert(make_pair(x, w));
                }
                cout << "     going to delete " << v << ", " << w;
                edges_to_delete.insert(make_pair(v, w)); 
                cout << '\n';
        }

        void finish()
        {
                cout << "finishing\n";
                cout << "edges to add size: " << edges_to_add.size() << '\n';
                cout << "adding: ";
                for( auto& p : edges_to_add    ){
                        assert(p.first != p.second);
                        cout << '(' << p.first << ", " << p.second << ")   ";
                        add_edge(p.first, p.second, *g);
                }
                cout << '\n';
                cout << "edges to remove size: " << edges_to_delete.size() << '\n';
                cout << "removing: ";
                for( auto& p : edges_to_delete ){ 
                        cout << '(' << p.first << ", " << p.second << ")   ";
                        remove_edge(p.first, p.second, *g);
                }
                cout << '\n';
        }

        void scan_nonsubtree_edges(VertDesc v, Graph const& g, Embedding const& em, BFSVisitorData const& bfs)
        {
                auto v_it = bfs.verts.find(v);
                assert(v_it != bfs.verts.end());
                if( v_it->second.level > l0 ) return;
                for( auto e : em[v] ){
                        auto src = source(e, g);
                        auto tar = target(e, g);
                        if( src == tar ) continue; // ?????

                        if( !bfs.is_tree_edge(e) ){
                                foundedge(v, e);
                                continue;
                        }
                        if( src != v ) swap(src, tar);
                        assert(src == v); 
                        auto tar_it = bfs.verts.find(tar);
                        if( tar_it->second.level > l0 ) foundedge(v, e);
                }
                //cout << "looking for children of " << v << '\n';
                auto vvv   = bfs.children.find(v);
                if( vvv == bfs.children.end() ) return; // no children
                //cout << "from " << v << " looking for children\n";
                for( auto& c : vvv->second ){
                        //cout << "child: " << c << '\n'; 
                        scan_nonsubtree_edges(c, g, em, bfs);
                }
        }

};

void reset_vertex_indices(Graph& g)
{
        VertIter vi, vend;
        uint i = 0;
        for( tie(vi, vend) = vertices(g); vi != vend; ++vi, ++i ) put(vertex_index, g, *vi, i); 
}

struct Em
{
        unique_ptr<EmbeddingStorage> storage;
        unique_ptr<Embedding>        em;
        Graph*            	     g;

        Em(Graph* g) : g(g), storage(new EmbeddingStorage(num_vertices(*g))), em(new Embedding(storage->begin())) 
        {
                testplanar();
        } 

        bool testplanar() {return boyer_myrvold_planarity_test(boyer_myrvold_params::graph = *g, boyer_myrvold_params::embedding = *em);}

        void print()
        {
                cout << CYAN << "\n************** Embedding ************\n" << RESET;
                VertIter vi, vend;
                for( tie(vi, vend) = vertices(*g); vi != vend; ++vi ){
                        cout << "vert " << *vi << ": ";
                        Embedding& embedding = *em;
                        for( auto ei = embedding[*vi].begin(); ei != embedding[*vi].end(); ++ei ){
                                auto src = source(*ei, *g);
                                auto tar = target(*ei, *g);
                                if( tar == *vi ) swap(src, tar);
                                cout << tar << ' ';
                        }
                        cout << "\n"; 
                }
                cout << CYAN << "*************************************\n" << RESET;
        }
};

EdgeIndex reset_edge_index(Graph const& g)
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
        Em em(&g);
        em.testplanar();
        make_biconnected_planar(g, *em.em, index);

        reset_edge_index(g);
        em.testplanar();

        make_maximal_planar(g, *em.em);

        reset_edge_index(g);
        assert(em.testplanar());
} 

vector<VertDesc> ancestors(VertDesc v, BFSVisitorData const& vis)
{
        //cout << "first v: " << v << '\n';
        //cout << "root: " << vis.root << '\n';
        vector<VertDesc> ans = {v};
        while( v != vis.root ){
                auto v_it = vis.verts.find(v);
                assert(v_it != vis.verts.end());
                v = v_it->second.parent;
                ans.push_back(v);
                //cout << "pushing back v: " << v << '\n';
        }
        return ans;
}

VertDesc common_ancestor(vector<VertDesc> const& ancestors_v, vector<VertDesc> const& ancestors_w, BFSVisitorData const& vis)
{
        uint i, j;
        for( i = 0; i < ancestors_v.size(); ++i ) for( j = 0; j < ancestors_w.size(); ++j ) if( ancestors_v[i] == ancestors_w[j] ) return ancestors_v[i];
        assert(0);
        return ancestors_v[i];
}

vector<VertDesc> get_cycle(VertDesc v, VertDesc w, VertDesc ancestor, BFSVisitorData const& vis_data)
{
        vector<VertDesc> cycle, tmp;
        VertDesc cur;
        cur = v; while( cur != ancestor ){ cycle.push_back(cur); auto cur_it = vis_data.verts.find(cur); cur = cur_it->second.parent; } cycle.push_back(ancestor); 
        cur = w; while( cur != ancestor ){ tmp  .push_back(cur); auto cur_it = vis_data.verts.find(cur); cur = cur_it->second.parent; }
        reverse(STLALL(tmp));
        cycle.insert(cycle.end(), STLALL(tmp));
        return cycle;
}

vector<VertDesc> get_cycle(VertDesc v, VertDesc w, BFSVisitorData const& vis_data)
{ 
        auto parents_v   = ancestors(v, vis_data);
        auto parents_w   = ancestors(w, vis_data); 
        auto ancestor    = common_ancestor(parents_v, parents_w, vis_data);
        cout << "common ancestor: " << ancestor << '\n'; 
        return get_cycle(v, w, ancestor, vis_data);
}

void kill_vertex(VertDesc v, Graph& g)
{
        cout << "killing vertex " << v << '\n';
        auto i = vert2uint[v];
        uint2vert.erase(i);
        vert2uint.erase(v);
        clear_vertex(v, g);
        remove_vertex(v, g);
}

EdgeDesc arbitrary_nontree_edge(Graph const& g, BFSVisitorData const& vis_data)
{ 
        EdgeIter ei, ei_end;
        for( tie(ei, ei_end) = edges(g); ei != ei_end; ++ei ){
                auto src = source(*ei, g);
                auto tar = target(*ei, g);
                assert(edge(src, tar, g).second); // exists
                assert(src != tar); 
                if( !vis_data.is_tree_edge(*ei) ) break;
        }
        assert(ei != ei_end);
        assert(!vis_data.is_tree_edge(*ei));
        EdgeDesc chosen_edge = *ei;
        cout << "arbitrarily choosing nontree edge: " << to_string(chosen_edge, g) << '\n';
        return chosen_edge;
}

set<VertDesc> get_neighbors(VertDesc v, Graph const& g)
{ 
        set<VertDesc> neighbors;
        OutEdgeIter e_cur, e_end;
        for( tie(e_cur, e_end) = out_edges(v, g); e_cur != e_end; ++e_cur ){
		auto ne = target(*e_cur, g);
		neighbors.insert(ne);
		cout << "      vertex " << v << " has neighbor " << ne << '\n';
	}
        return neighbors;
}

set<VertDesc> get_intersection(set<VertDesc> const& a, set<VertDesc> const& b)
{
        set<VertDesc> c;
        set_intersection(STLALL(a), STLALL(b), inserter(c, c.begin())); 
        for( auto& i : c ) cout << "      set intersection: " << i << '\n'; 
        assert(c.size() == 2);
        return c;
} 

struct CycleCost
{
        uint inside = 0;
	uint outside = 0;
};

CycleCost compute_cycle_cost(vector<VertDesc> const& cycle, Graph const& g, BFSVisitorData const& vis_data, Em const& em)
{
        CycleCost cc;
        for( auto& v : cycle ){
                //cout << "   scanning cycle vert " << v << '\n';
                for( auto e = out_edges(v, g); e.first != e.second; ++e.first ) if( vis_data.is_tree_edge(*e.first) && !on_cycle(*e.first, cycle, g) ){
                        uint cost = vis_data.edge_cost(*e.first, cycle, g);
                        //cout << "      scanning incident tree edge " << to_string(*e.first, g) << "   cost: " << cost << '\n';
                        auto insideout = edge_inside_cycle(*e.first, v, cycle, g, *em.em);
                        assert(insideout != ON);
                        bool is_inside = (insideout == INSIDE);
                        (is_inside ? cc.inside : cc.outside) += cost;
                }
        }
        return cc;
}

Partition construct_vertex_partition(Graph const& g_orig, int l[3], BFSVisitorData& vis_data)
{
        cout << HEADER_COL << "\n------------ 10  - Construct Vertex Partition --------------\n" << RESET;
        print_graph(g_orig, false);
        cout << "l0: " << l[0] << '\n';
        cout << "l1: " << l[1] << '\n';
        cout << "l2: " << l[2] << '\n';

        uint r = vis_data.num_levels;
        cout << "r: " << r << '\n';

        if( l[1] >= l[2] ){ 
                cout << MAGENTA << "l1 is less than l2\n" << RESET; 
                vector<VertDesc> part_a, part_b, part_c;
                VertIter vei, vend;
                for( tie(vei, vend) = vertices(g_orig); vei != vend; ++vei ){ 
                        auto v = *vei;
                        cout << "level of " << v << ": " << vis_data.verts[v].level << "  ";
                        if( vis_data.verts[v].level <  l[1] )                                  { cout << v << " belongs to first part\n";  part_a.push_back(v); continue; }
                        if( vis_data.verts[v].level >= l[1]+1 && vis_data.verts[v].level <= r ){ cout << v << " belongs to middle part\n"; part_b.push_back(v); continue; }
                        if( vis_data.verts[v].level == l[1] )                                  { cout << v << " belongs to last part\n";   part_c.push_back(v); continue; }
                        assert(0);
                } 
                cout << GREEN;
                cout <<   "A = all verts on levels 0    thru l1-1: "; for( auto& a : part_a ) cout << a << ' ';
                cout << "\nB = all verts on levels l1+1 thru r   : "; for( auto& b : part_b ) cout << b << ' ';
                cout << "\nC = all verts on llevel l1            : "; for( auto& c : part_c ) cout << c << ' ';
                cout << RESET;
                return empty_partition;
        } 

        vector<VertDesc> part_a, part_b, part_c, deleted_part;
        VertIter vei, vend;
        for( tie(vei, vend) = vertices(g_orig); vei != vend; ++vei ){ 
                auto v = *vei;
                cout << "level of " << v << ": " << vis_data.verts[v].level << ", ";
                if( vis_data.verts[v].level == l[1] || vis_data.verts[v].level == l[2] ){     cout << v << " is deleted\n";             deleted_part.push_back(v); continue;}
                if( vis_data.verts[v].level <  l[1] ){                                        cout << v << " belongs to first part\n";  part_a.push_back(v);       continue;}
                if( vis_data.verts[v].level >= l[1]+1 && vis_data.verts[v].level <= l[2]-1 ){ cout << v << " belongs to middle part\n"; part_b.push_back(v);       continue;}
                if( vis_data.verts[v].level >  l[2]  ){                                       cout << v << " belongs to last part\n";   part_c.push_back(v);       continue;}
                assert(0);
        }

        //the only part which can have cost > 2/3 is the middle part
        assert(part_a.size() <= 2*num_vertices(g_orig)/3);
        assert(part_c.size() <= 2*num_vertices(g_orig)/3);
        if( part_b.size() <= 2*num_vertices(g_orig)/3 ){
                cout << MAGENTA << "middle part NOT biggest\n" << RESET;
                vector<VertDesc>* costly_part, * other1, * other2;
                if( part_a.size() > part_b.size() && part_a.size() > part_c.size() ){ costly_part = &part_a; other1 = &part_b; other2 = &part_c; cout << "part a is most costly\n";}
                if( part_b.size() > part_a.size() && part_b.size() > part_c.size() ){ costly_part = &part_b; other1 = &part_a; other2 = &part_c; cout << "part b is most costly\n";}
                if( part_c.size() > part_a.size() && part_c.size() > part_b.size() ){ costly_part = &part_c; other1 = &part_a; other2 = &part_b; cout << "part c is most costly\n";}
                cout << "part a size: " << part_a.size() << '\n';
                cout << "part b size: " << part_b.size() << '\n';
                cout << "part c size: " << part_c.size() << '\n';
                cout <<   "A = most costly part of the 3: "; for( auto& a : *costly_part ) cout << a << ' ';
                cout << "\nB = remaining 2 parts        : "; for( auto& b : *other1      ) cout << b << ' '; for( auto& b : *other2 ) cout << b << ' '; 
                cout << "\nC =                          : "; for( auto& v : deleted_part ) cout << v << ' '; cout << '\n';
        } else {
                cout << MAGENTA << "middle part biggest\n" << RESET;
                //delete all verts on level l2 and above
                //shrink all verts on levels l1 and belowe to a single vertex of cost zero
                //The new graph has a spanning tree radius of l2 - l1 -1 whose root corresponds to vertices on levels l1 and below in the original graph
                r = l[2] - l[1] - 1;
                //Apply Lemma 2 to the new graph, A* B* C*
                cout << "A = set among A* and B* with greater cost\n";
                cout << "C = verts on levels l1 and l2 in the original graph plus verts in C* minus the root\n";
                cout << "B = remaining verts\n";
                //By Lemma 2, A has total cost <= 2/3
                //But A U C* has total cost >= 1/3, so B also has total cost <= 2/3
                //Futhermore, C contains no more than L[l1] + L[l2] + 2(l2 - l1 - 1)
        }
        return empty_partition;
}

Partition improve_separator(Graph const& g, Graph const& g_orig, CycleCost& cc, EdgeDesc chosen_edge, BFSVisitorData& vis_data, vector<VertDesc> const& cycle, Em const& em2, bool cost_swapped, int l[3])
{
        cout << HEADER_COL << "---------------------------- 9 - Improve Separator -----------\n" << RESET;
        print_edges(g);

        while( cc.inside > num_vertices(g)*2./3 ){ 
                cout << RED << "chosen_edge: " << to_string(chosen_edge, g) << '\n';
                cout << "const inside: " << cc.inside  << '\n';
                cout << "const outide: " << cc.outside << '\n';
                cout << "looking for a better cycle\n" << RESET;

                auto vi = source(chosen_edge, g);
                auto wi = target(chosen_edge, g);
                assert(!vis_data.is_tree_edge(chosen_edge));
                EdgeDesc next_edge;
                cout << "   vi: " << vi << '\n';
                cout << "   wi: " << wi << '\n';

                auto neighbors_v = get_neighbors(vi, g);
                auto neighbors_w = get_neighbors(wi, g); 
                auto intersect   = get_intersection(neighbors_v, neighbors_w); 
                assert(intersect.size() == 2);
                cout << "   intersectbegin: " << *intersect.begin() << '\n';

                auto eee = edge(vi, *intersect.begin(), g);
                cout << "eee: " << to_string(eee.first, g) << '\n';
                assert(eee.second);

                InsideOutOn insideout = edge_inside_cycle(eee.first, *intersect.begin(), cycle, g, *em2.em);
                auto y = (insideout == INSIDE) ? *intersect.begin() : *(++intersect.begin());

                cout << "   y: " << y << '\n';
                auto viy_e = edge(vi, y, g); assert(viy_e.second); auto viy = viy_e.first;
                auto ywi_e = edge(y, wi, g); assert(ywi_e.second); auto ywi = ywi_e.first; 
                if ( vis_data.is_tree_edge(viy) || vis_data.is_tree_edge(ywi) ){
                        cout << MAGENTA << "   at least one tree edge\n" << RESET;
                        next_edge = vis_data.is_tree_edge(viy) ? ywi : viy;
                        assert(!vis_data.is_tree_edge(next_edge));

                        // Compute the cost inside the (vi+1 wi+1) cycle from the cost inside the (vi, wi) cycle and the cost of vi, y, and wi.  See Fig 4.
                        uint cost1 = vis_data.verts[vi].descendant_cost;
                        uint cost2 = vis_data.verts[y ].descendant_cost;
                        uint cost3 = vis_data.verts[wi].descendant_cost;
                        uint cost4 = cc.inside;
                        auto new_cycle = get_cycle(source(next_edge, g), target(next_edge, g), vis_data);
                        cc = compute_cycle_cost(new_cycle, g, vis_data, em2); // !! CHEATED !!
                        if( cost_swapped ) swap(cc.outside, cc.inside);
                } else {
                        // Determine the tree path from y to the (vi, wi) cycle by following parent pointers from y.
                        cout << MAGENTA << "   neither are tree edges\n" << RESET;
                        auto path = ancestors(y, vis_data);
                        uint i;
                        for( i = 0; !on_cycle(path[i], cycle, g); ++i );

                        // Let z be the vertex on the (vi, wi) cycle reached during the search.
                        auto z = path[i++];
                        cout << "    z: " << z << '\n';
                        path.erase(path.begin()+i, path.end());
                        assert(path.size() == i);

                        // Compute the total cost af all vertices except z on this tree path.
                        uint path_cost = path.size() - 1;
                        cout << "    y-to-z-minus-z cost: " << path_cost << '\n';

                        // Scan the tree edges inside the (y, wi) cycle, alternately scanning an edge in one cycle and an edge in the other cycle.
                        // Stop scanning when all edges inside one of the cycles have been scanned.  Compute the cost inside this cycle by summing the associated costs of all scanned edges.
                        // Use this cost, the cost inside the (vi, wi) cycle, and the cost on the tree path from y to z to compute the cost inside the other cycle.
                        auto cycle1 = get_cycle(vi, y, vis_data);
                        auto cycle2 = get_cycle(y, wi, vis_data);

                        auto cost1  = compute_cycle_cost(cycle1, g, vis_data, em2);
                        auto cost2  = compute_cycle_cost(cycle2, g, vis_data, em2);
                        if( cost_swapped ){
                                swap(cost1.inside, cost1.outside);
                                swap(cost2.inside, cost2.outside);
                        }

                        // Let (vi+1, wi+1) be the edge among (vi, y) and (i, wi) whose cycle has more cost inside it.
                        if( cost1.inside > cost2.inside ){ next_edge = edge(vi, y, g).first; cc = cost1; }
                        else                             { next_edge = edge(y, wi, g).first; cc = cost2; }
                } 
                chosen_edge = next_edge;
        }
        cout << "found cycle with inside cost < 2/3: " << cc.inside << '\n';
        print_cycle(cycle);

	return construct_vertex_partition(g_orig, l, vis_data);
} 

struct NotPlanar {}; 

Partition locate_cycle(Graph& g, Graph& g_orig, BFSVisitorData& vis_data, int l[3])
{
        cout << HEADER_COL << "----------------------- 8 - Locate Cycle -----------------\n" << RESET; 
        auto chosen_edge = arbitrary_nontree_edge(g, vis_data);
        auto v1          = source(chosen_edge, g);
        auto w1          = target(chosen_edge, g); 
        cout << "ancestors v1...\n";
        auto parents_v   = ancestors(v1, vis_data);
        cout << "ancestors v2...\n";
        auto parents_w   = ancestors(w1, vis_data); 
        auto ancestor    = common_ancestor(parents_v, parents_w, vis_data);
        cout << "common ancestor: " << ancestor << '\n'; 
        auto cycle = get_cycle(v1, w1, ancestor, vis_data);

        Em   em2(&g);
        auto cc = compute_cycle_cost(cycle, g, vis_data, em2); 
	bool cost_swapped;
        if( cc.outside > cc.inside ){
                swap(cc.outside, cc.inside);
                cost_swapped = true;
                cout << "!!!!!! cost swapped !!!!!!!!\n";
        } else cost_swapped = false;
        cout << "total inside cost:  " << cc.inside  << '\n'; 
        cout << "total outside cost: " << cc.outside << '\n';

	return improve_separator(g, g_orig, cc, chosen_edge, vis_data, cycle, em2, cost_swapped, l);
}

Partition new_bfs_and_make_max_planar(Graph& g, Graph& g_orig, BFSVisitorData& vis_data, VertDesc x_gone, VertDesc x, int l[3])
{
        cout << HEADER_COL << "-------------------- 7 - New BFS and Make Max Planar -----\n" << RESET;
        reset_vertex_indices(g);
        reset_edge_index(g);
        vis_data.reset(&g);
        vis_data.root = (x_gone != Graph::null_vertex()) ? x_gone : x;
        ++vis_data.verts[vis_data.root].descendant_cost;

        cout << "root: " << vis_data.root << '\n'; 
        cout << "n:    " << num_vertices(g) << '\n';

        breadth_first_search(g, x_gone != Graph::null_vertex() ? x_gone: x, visitor(BFSVisitor(vis_data))); 
        makemaxplanar(g);
        reset_vertex_indices(g);
        reset_edge_index(g);

        print_graph(g);

	return locate_cycle(g, g_orig, vis_data, l); 
}

Partition shrinktree(Graph& g, Graph& g_orig, VertIter vit, VertIter vjt, BFSVisitorData& vis_data, int l[3])
{
        cout << HEADER_COL << "---------------------------- 6 - Shrinktree -------------\n" << RESET;
        cout << "n: " << num_vertices(g) << '\n'; 

        vector<VertDesc> replaceverts;
        tie(vit, vjt) = vertices(g); 
        for( auto next = vit; vit != vjt; vit = next ){
                ++next;
                if( vis_data.verts[*vit].level >= l[2] ){
                        cout << "deleting vertex " << *vit << " of level l2 " << vis_data.verts[*vit].level << " >= " << l[2] << '\n';
                        kill_vertex(*vit, g);
                }
                if( vis_data.verts[*vit].level <= l[0] ){
                        cout << "going to replace vertex " << *vit << " of level l0 " << vis_data.verts[*vit].level << " <= " << l[0] << '\n';
                        replaceverts.push_back(*vit);
                }
        }

        auto x = add_vertex(g); uint2vert[vert2uint[x] = 999999] = x; 
        map<VertDesc, bool> t;
        for( tie(vit, vjt) = vertices(g); vit != vjt; ++vit ){
                t[*vit] = vis_data.verts[*vit].level <= l[0];
                cout << "vertex " << *vit << " at level " << vis_data.verts[*vit].level << " is " << (t[*vit] ? "TRUE" : "FALSE") << '\n';
        }

        reset_vertex_indices(g);
        reset_edge_index(g);
        Em em(&g);
        assert(em.testplanar());

        ScanVisitor svis(&t, &g, x, l[0]);
        svis.scan_nonsubtree_edges(*vertices(g).first, g, *em.em, vis_data);
        svis.finish();

        auto x_gone = Graph::null_vertex();
        if( !degree(x, g) ){
                cout << "no edges to x found, deleting\n";
                kill_vertex(x, g);
                x_gone = *vertices(g).first;
                cout << "x_gone: " << x_gone << '\n';
        } else {
                // delete all vertices x has replaced
                for( auto& v : replaceverts ) kill_vertex(v, g);
        }

	return new_bfs_and_make_max_planar(g, g_orig, vis_data, x_gone, x, l);
}

Partition lipton_tarjan(Graph& g, Graph& g_orig)
{
        cout << HEADER_COL << "---------------------------- 1 - Check Planarity  ------------\n" << RESET;
        Em em1(&g);
        if( !em1.testplanar() ) throw NotPlanar();
        cout << "planar ok\n";
        print_graph(g);

        cout << HEADER_COL << "---------------------------- 2 - Connected Components --------\n" << RESET;
        VertDescMap idx; 
        associative_property_map<VertDescMap> vertid_to_component(idx);
        VertIter vit, vjt;
        tie(vit, vjt) = vertices(g);
        for( uint i = 0; vit != vjt; ++vit, ++i ) put(vertid_to_component, *vit, i);
        uint components = connected_components(g, vertid_to_component);

        cout << "# of components: " << components << '\n';
        vector<uint> verts_per_comp(components, 0);
        for( tie(vit, vjt) = vertices(g); vit != vjt; ++vit ) ++verts_per_comp[vertid_to_component[*vit]];
        uint biggest_component = 0;
        uint biggest_size      = 0;
        bool too_big           = false;
        for( uint i = 0; i < components; ++i ){
                if( 3*verts_per_comp[i] > 2*num_vertices(g) ){
                        cout << "too big\n";
                        too_big = true;
                }
                if( verts_per_comp[i] > biggest_size ){
                        biggest_size = verts_per_comp[i];
                        biggest_component = i;
                }
        }

        if( !too_big ){
                theorem4(0, g);
                return empty_partition;
        }
        cout << "biggest component: " << biggest_component << '\n';

        cout << HEADER_COL << "---------------------------- 3 - BFS and Levels ------------\n" << RESET;
        BFSVisitorData vis_data(&g);
        auto root = *vertices(g).first;
        vis_data.root = root;
        breadth_first_search(g, root, visitor(BFSVisitor(vis_data)));

        vector<uint> L(vis_data.num_levels + 1, 0);
        for( auto& d : vis_data.verts ) ++L[d.second.level];

        for( tie(vit, vjt) = vertices(g); vit != vjt; ++vit ) cout << "level/cost of vert " << *vit << ": " << vis_data.verts[*vit].level << '\n';
        for( uint i = 0; i < L.size(); ++i ) cout << "L[" << i << "]: " << L[i] << '\n';

        cout << HEADER_COL << "---------------------------- 4 - l1 and k  ------------\n" << RESET;
        uint k = L[0]; 
        int l[3];
        l[1] = 0;
        while( k <= num_vertices(g)/2 ) k += L[++l[1]];
        cout << "k:  " << k    << "      # of verts in levels 0 thru l1\n";
        cout << "l1: " << l[1] << "      total cost of levels 0 thru l1 barely exceeds 1/2\n";

        cout << HEADER_COL << "---------------------------- 5 - Find More Levels -------\n" << RESET;
        float sq  = 2 * sqrt(k); 
        float snk = 2 * sqrt(num_vertices(g) - k); 
        cout << "sq:    " << sq << '\n';
        cout << "snk:   " << snk << '\n';

        l[0] = l[1];     for( ;; ){ float val = L.at(l[0]) + 2*(l[1] - l[0]);     if( val <= sq  ) break; --l[0]; } cout << "l0: " << l[0] << "     highest level <= l1\n";
        l[2] = l[1] + 1; for( ;; ){ float val = L.at(l[2]) + 2*(l[2] - l[1] - 1); if( val <= snk ) break; ++l[2]; } cout << "l2: " << l[2] << "     lowest  level >= l1 + 1\n";

	return shrinktree(g, g_orig, vit, vjt, vis_data, l);
}
