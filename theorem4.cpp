#include "theorem4.h"
#include "strutil.h"
#include "graphutil.h"
#include "lemmas.h"
#include "BFSVisitor.h"
#include <boost/graph/copy.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/is_straight_line_drawing.hpp>
#include <boost/graph/make_maximal_planar.hpp>
#include <boost/graph/copy.hpp>
#include <boost/bimap.hpp>
#include <boost/config.hpp>
#include <iostream>
using namespace std;
using namespace boost;

// may be able to inline this and hoist it out of the if blocks of theorem4_disconnected if we're able to get rid of bigger_than_two_thirds
uint lowest_i(uint n, uint num_components, vector<uint> const& num_verts_per_component)
{
        uint total_cost = 0;
        uint i = 0;
        for( ; i < num_components; ++i ){
                total_cost += num_verts_per_component[i];
                if ( total_cost*3 > n ) return i;
        } 

        return -1;
}


// L[l] = # of vertices on level l
Partition theorem4_connected(GraphCR g, vector<uint> const& L, uint l[3], uint r, Graph* g_shrunk, BFSVisitorData const* bfs2)
{
        uint n = num_vertices(g);
        cout << "g:\n";
        print_graph(g);
        vertex_t v;

        // l1 = the level such that the sum of costs in levels 0 thru l1-1 < 1/2, but the sum of costs in levels 0 thru l1 is >= 1/2
        uint total = 0;
        uint level = 0;
        while( total < n/2 ){ 
                total += L[level++];
        }

        uint l1 = level;

        assert(1 == L[0]);
        // Partition the vertices into levels according to their distance from some vertex v.
        //uint r;
        /*If r is the maximum distance of any vertex from v, define additional levels -1 and r+1 containing no vertices*/
        //uint l[3];
        //(If no such l1 exists, the total cost of all vertices < 1/2, and B = C = {} and return true) */
        uint k = 0;// = # of vertices on levels 0 thru l1.
        for( uint i = 0; i <= l1; ++i ) k += L[i];

        /*Find a level l0 such that l0 <= l1 and |L[l0]| + 2(l1-l0) <= 2sqrt(k)
        Find a level l2 such that l1+1 <= l2 and |L[l2] + 2(l2-l1-1) <= 2sqrt(n-k) */
        uint sqrtk = 2*sqrt(k);
        uint sqrtnk = 2*sqrt(n-k);

        uint l0 = l1;
        while( L[l0] + 2*(l1-l0) > sqrtk ) --l0;

        uint l2 = l1+1;
        while( L[l2] + 2*(l2-l1-1) > sqrtnk) ++l2;
        
        assert(l0 <= l1);
        assert(l1+1 <= l2);

        BFSVisitorData bfs(&g, *vertices(g).first);
        breadth_first_search(g, bfs.root, boost::visitor(BFSVisitor(bfs)));
        cout << "bfsroot: " << bfs.root << '\n';

        vector<vertex_t> cycle;
        l1 -= 2;
        --l2;
        Partition p = lemma3(g, L, l1, l2, r, bfs, *bfs2, cycle, g_shrunk);

        uint climit = sqrtk - sqrtnk;
        assert(p.verify_edges(g));
        //assert(p.verify_sizes_lemma3(L, l1, l2));

        /*If 2 such levels exist, then by Lemma 3 the vertices of G can be partitioned into three sets A, B, C such that no edge joins a vertex in A with a vertex in B,
        neither A or C has cost > 2/3, and C contains no more than 2(sqrt(k) + sqrt(n-k)) vertices.
        But 2(sqrt(k) + sqrt(n-k) <= 2(sqrt(n/2) + sqrt(n/2)) = 2sqrt(2)sqrt(n)
        Thus the theorem holds if suitable levels l0 and l2 exist
                Suppose a suitable level l0 does not exist.  Then, for i <= l1, L[i] >= 2sqrt(k) - 2(l1-i)
                Since L[0] = 1, this means 1 >= 2sqrt(k) - 2l1 and l1 + 1/2 >= sqrt(k).  Thus l1 = floor(l1 + 1/2) > 
                Contradiction*/

        return p; 
}


Partition theorem4_ccbigger23(GraphCR g_all, Partition const& biggest_comp_p)
{ 
        uint alln = num_vertices(g_all);
        cout << "alln: " << alln << '\n';

        Graph g_comp(g_all); // connected component

        auto g_all_prop_map = get(vertex_index, g_all);
        auto g_comp_prop_map = get(vertex_index, g_comp);

        auto vertid_to_vert_t = [&](uint id, decltype(get(vertex_index, g_all)) prop_map)
        { 
                VertIter vit, vjt;
                tie(vit, vjt) = vertices(g_all); 
                for( ; vit != vjt; ++vit ){
                        if( prop_map[*vit] == id ) return *vit; 
                } 
                return Graph::null_vertex();
        };

        // for all vertices v in g_comp,
        vector<vertex_t> toremove;
        VertIter vit, vjt;
        tie(vit, vjt) = vertices(g_comp); 
        for( ; vit != vjt; ++vit ){
                vertex_t v = *vit;
                cout << "currently examining " << v << " comp_propmap" << g_comp_prop_map[v] << '\n';
                // cv = lookup the corresponding vertex in g_all
                vertex_t cv = vertid_to_vert_t(g_all_prop_map[v], g_all_prop_map);
                cout << "       with cv " << cv << " all_propmap" << g_all_prop_map[v] << '\n';
                // if cv is not in biggest_comp_p,
                if( !biggest_comp_p.a.contains(cv) && 
                    !biggest_comp_p.b.contains(cv) && 
                    !biggest_comp_p.c.contains(cv) ){ 
                        // delete v from g_comp
                            toremove.push_back(v);
                    }

        }

        cout << "gcomp:\n";
        print_graph(g_comp);
        print_graph_addresses(g_comp);

        uint nremove = toremove.size();
        cout << "nremove: " << nremove << '\n';
        for(auto& v : toremove ){
                cout << "   nr: " << v << '\n'; 
                kill_vertex(v, g_comp); 
        }

        reset_vertex_indices(g_comp);


        cout << "gcomp:\n";
        print_graph(g_comp);
        print_graph_addresses(g_comp);
        BFSVisitorData vd(&g_comp, *vertices(g_comp).first);
        breadth_first_search(g_comp, vd.root, boost::visitor(BFSVisitor(vd)));

        // disabled because they don't support multiple connected components
        //assert(assert_verts(g_orig, vis_data_orig));
        //assert(assert_verts(g_copy, vis_data_copy));

        uint n = num_vertices(g_comp);

        vector<uint> L(vd.num_levels + 1, 0);
	cout << "L levels: " << L.size() << '\n';
        for( auto& d : vd.verts ){
                cout << "level: " << d.second.level << '\n';
	       	++L[d.second.level];
	}

        uint k = L[0]; 
        uint l[3];
        l[1] = 0;
        while( k <= n/2 ){
                uint indx = ++l[1];
                uint lsize = L.size();
                if( indx >= lsize ) break;
	       	k += L.at(indx);
	}

        float sq  = 2 * sqrt(k); 
        float snk = 2 * sqrt(num_vertices(g_comp) - k);
        cout << "sq:     " << sq << '\n';
        cout << "snk:    " << snk << '\n';
        cout << "L size: " << L.size() << '\n';

        l[0] = l[1];
        cout << "l[0]:   " << l[0] << '\n';
        while( l[0] < L.size() ){
                float val = L.at(l[0]) + 2*(l[1] - l[0]);
                if( val <= sq ) break;
                --l[0];
        }
        cout << "l0: " << l[0] << "     highest level <= l1\n";

        l[2] = l[1] + 1;
        cout << "l[2]" << l[2] << '\n';
        while( l[2] < L.size() ){
                float val = L.at(l[2]) + 2*(l[2] - l[1] - 1);
                if( val <= snk ) break;
                ++l[2];
        }
        cout << "l2: " << l[2] << "     lowest  level >= l1 + 1\n";

        uint r = vd.num_levels - 1;

        Partition star_p = theorem4_connected(g_comp, L, l, r, nullptr, nullptr);
        // ????

        Partition p;
        p.c = star_p.c;
        p.b.insert(STLALL(toremove));
        if( star_p.a.size() > star_p.b.size() ){
                p.a = star_p.a; 
                p.b.insert(STLALL(star_p.b));
        } else {
                p.a = star_p.b; 
                p.b.insert(STLALL(star_p.a));
        }
        return p; 
}

Partition theorem4_disconnected(GraphCR g, uint n, uint num_components, associative_property_map<vertex_map> const& vertid_to_component, vector<uint> const& num_verts_per_component, Partition const& biggest_comp_p)
{
        vector<vector<VertIter>> vertex_sets; // set of vertex ids, indexed by component number (second vector should be set but compiler did not like call to .insert())

        vertex_sets.resize(num_components);

        // populate vertex sets
        VertIter vit, vjt;
        tie(vit, vjt) = vertices(g);
        for( uint i = 0; vit != vjt; ++vit, ++i ){
                uint component = vertid_to_component[*vit];
                vector<VertIter>& vset = vertex_sets[component];
                vset.push_back(vit);
        }

        // Let G1, G2, ... , Gk be the connected components of G, with vertex sets V1, V2, ... , Vk respectively.
        bool bigger_than_one_third = false;
        bool bigger_than_two_thirds = false;
        for( uint i = 0; i < num_components; ++i ){
                if( num_verts_per_component[i] > n    /3.0 ) bigger_than_one_third  = true;
                if( num_verts_per_component[i] > n*2.0/3.0 ){ bigger_than_two_thirds = true; break;}
        }

        if( !bigger_than_one_third ){ 

                // If no connected component has total vertex cost > 1/3, let i be the minimum index such that the total cost of V1 U V2 U ... U Vi > 1/3
                int i = lowest_i(n, num_components, num_verts_per_component); // -1 indicates no such index found

                Partition p;

                // populate partition A = V1 U V2 U ... U Vi 
                for( int j = 0; j <= i; ++j ){
                        vector<VertIter>& vec = vertex_sets[j];
                        for( VertIter& v : vec ) p.a.insert(*v);
                }

                // populate partition B = Vi+1 U Vi+2 U ... U Vk
                for( int j = i+1; j < (int)num_components; ++j ){
                        vector<VertIter>& vec = vertex_sets[j];
                        for( VertIter& v : vec ) p.b.insert(*v);
                }

                // Since i is minimum and the cost of Vi <= 1/3, the cost of A <= 2/3. return true;
                cout << "not bigger than one third\n";
                return p;
        } else if( !bigger_than_two_thirds ){
                // If some connected component (say Gi) has total vertex cost between 1/3 and 2/3,
                Partition p; 

                int i = lowest_i(n, num_components, num_verts_per_component);
                assert(i >= 0 && num_verts_per_component[i] >= n/3 && num_verts_per_component[i] <= 2*n/3);

                // populate partition A
                cout << "!! populating partition A\n";
                for( uint j = 0; j <= i; ++j ){
                        for( uint v = 0; v < vertex_sets[j].size(); ++v ){
                                p.a.insert(*vertex_sets[j][v]);
                        }

                }
                cout << '\n';

                // populate partition B, should be everything except what's in partition A
                cout << "!! populating partition B\n";
                for( uint j = i+1; j < num_components; ++j ){

                        vector<VertIter>& vset = vertex_sets[j];
                        for( VertIter& v : vset ){
                                p.b.insert(*v);
                                cout << "v: " << *v << endl;
                        }
                }

                // partition C should be empty 
                assert(p.c.empty());

                p.print(&g);

                cout << "bigger than one third but less than two thirds\n";
                return p;
        }

        return theorem4_ccbigger23(g, biggest_comp_p);
}

/* Theorem 4: Let G be any (possibly disconnected) n-vertex planar graph having nonnegative vertex costs summing to no more than one.
Then the vertices of G can be partitioned into three sets A, B, C such that no edge joins a vertex
in A with a vertex in B, neither A nor B has total cost exceeding 2/3, and C contains no more than
2sqrt(2)sqrt(n) vertices :*/
Partition theorem4(GraphCR g, associative_property_map<vertex_map> const& vertid_to_component, vector<uint> const& num_verts_per_component, Partition const& biggest_comp_p)
{
	uint n = num_vertices(g);
	uint num_components = num_verts_per_component.size();
	bool is_graph_connected = (num_components == 1);

	if( is_graph_connected ){
		cout << "graph is connected\n";
                vector<uint> L;
                uint l[3];
                uint r;
                assert(0);
                return theorem4_connected(g, L, l, r, nullptr, nullptr);
	} else {
                cout << "graph is disconnected with " << num_components << " components\n";
                return theorem4_disconnected(g, n, num_components, vertid_to_component, num_verts_per_component, biggest_comp_p);
	}
}
