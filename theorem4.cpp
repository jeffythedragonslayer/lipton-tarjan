#include "theorem4.h"
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
                if ( total_cost > n/3 ) return i;
        } 

        return -1;
}

// L[l] = # of vertices on level l
Partition theorem4_connected(GraphCR g, vector<uint> const& L, uint l[3], uint r)
{
        uint n = num_vertices(g);
        cout << "g:\n";
        print_graph(g);
        vertex_t v;

        uint l0 = l[0];
        uint l1 = l[1];
        uint l2 = l[2];
        // Partition the vertices into levels according to their distance from some vertex v.
        //uint r;
        /*If r is the maximum distance of any vertex from v, define additional levels -1 and r+1 containing no vertices*/
        //uint l[3];// l1 = the level such that the sum of costs in levels 0 thru l1-1 < 1/2, but the sum of costs in levels 0 thru l1 is >= 1/2
        //(If no such l1 exists, the total cost of all vertices < 1/2, and B = C = {} and return true) */
        uint k = 0;// = # of vertices on levels 0 thru l1.
        for( uint i = 0; i <= l1; ++i ) k += L[i];

        /*Find a level l0 such that l0 <= l1 and |L[l0]| + 2(l1-l0) <= 2sqrt(k)
        Find a level l2 such that l1+1 <= l2 and |L[l2] + 2(l2-l1-1) <= 2sqrt(n-k) */
        assert(l0 <= l1);
        assert(l1+1 <= l2);
        assert(L[l0] + 2*(l1-l0) <= 2*sqrt(k));
        assert(L[l2] + 2*(l2-l1-1) <= 2*sqrt(n-k));

        BFSVisitorData bfs(&g, *vertices(g).first);
        breadth_first_search(g, bfs.root, boost::visitor(BFSVisitor(bfs)));
        cout << "bfsroot: " << bfs.root << '\n';

        vector<vertex_t> cycle;
        Partition p = lemma3(g, L, l1, l2, r, bfs, bfs, cycle);

        /*If 2 such levels exist, then by Lemma 3 the vertices of G can be partitioned into three sets A, B, C such that no edge joins a vertex in A with a vertex in B,
        neither A or C has cost > 2/3, and C contains no more than 2(sqrt(k) + sqrt(n-k)) vertices.
        But 2(sqrt(k) + sqrt(n-k) <= 2(sqrt(n/2) + sqrt(n/2)) = 2sqrt(2)sqrt(n)
        Thus the theorem holds if suitable levels l0 and l2 exist
                Suppose a suitable level l0 does not exist.  Then, for i <= l1, L[i] >= 2sqrt(k) - 2(l1-i)
                Since L[0] = 1, this means 1 >= 2sqrt(k) - 2l1 and l1 + 1/2 >= sqrt(k).  Thus l1 = floor(l1 + 1/2) > 
                Contradiction*/

        return p; 
}

Partition theorem4_ccbigger23(GraphCR g, Partition const& biggest_comp_p)
{
        /*Finally, if some connected component (say Gi) has total vertex cost exceeding 2/3,
        apply the above argument to Gi
                Let A*, B*, C* be the resulting partition.
                A = set among A* and B* with greater cost
                C = C*
                B = remanining vertices of G
                Then A and B have cost <= 2/3, g
                return true

        In all cases the separator C is either empty or contained in only one connected component of G */

        Graph g_comp; // connected component
        copy_graph(g, g_comp);
        g_comp = g;
        // delete all verts not in the biggest connected component from g_comp

        vector<uint> L;
        uint l[3];
        uint r;

        Partition star_p = theorem4_connected(g_comp, L, l, r);
        // ????

        set<vertex_t> const* costly_part;
        set<vertex_t> const* other1;
        set<vertex_t> const* other2;
        star_p.get_most_costly_part(&costly_part, &other1, &other2);

        Partition p;
        p.a = *costly_part;
        p.c = star_p.c;
        p.b.insert(biggest_comp_p.a.begin(), biggest_comp_p.a.end());
        p.b.insert(biggest_comp_p.b.begin(), biggest_comp_p.b.end());
        p.b.insert(biggest_comp_p.c.begin(), biggest_comp_p.c.end());
        // set p.b to remaining vertices of G
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
                vector<VertIter>& vset = vertex_sets[i];
                cout << "vecsize: " << vset.size() << '\n';
                for( VertIter& v : vset ){
                        cout << "v: " << *v << endl;
                        p.a.insert(*v); 
                }
                cout << '\n';

                // populate partition B, should be everything except what's in partition A
                cout << "!! populating partition B\n";
                for( uint j = 0; j < num_components; ++j ){
                        if( i != j ){
                                vector<VertIter>& vset = vertex_sets[j];
                                for( VertIter& v : vset ){
                                        p.b.insert(*v);
                                        cout << "v: " << *v << endl;
                                }
                        }
                }

                // partition C should be empty 
                assert(p.c.empty());

                p.print();

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
                return theorem4_connected(g, L, l, r);
	} else {
                cout << "graph is disconnected with " << num_components << " components\n";
                return theorem4_disconnected(g, n, num_components, vertid_to_component, num_verts_per_component, biggest_comp_p);
	}
}
