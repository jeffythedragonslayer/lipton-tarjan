#include "lemmas.h"
#include "graphutil.h"
#include "strutil.h"
#include "Partition.h"
#include "BFSVisitor.h"
#include <boost/graph/copy.hpp>
#include <boost/graph/graph_utility.hpp>
using namespace std;
using namespace boost;

/* Let G be any planar graph with nonnegative vertex costs summing to no more than one.
Suppose G has a spanning tree of radius r.
Then the vertices of G can be partitioned into three sets A, B, C such that no edge joins a vertex in A with a vertex in B, neither A nor B has total cost exceeding 2/3,
and C contains no more than 2r+1 vertices, one the root of the tree */
// r is spanning tree radius
Partition lemma2(GraphCR g_orig, vector<vertex_t> const& cycle, BFSVisitorData const& visdata_orig)
{
        cout << "lemma2 graph:\n";
        print_graph(g_orig);
        uint n = num_vertices(g_orig);

        Graph g_shrink2;
        copy_graph(g_orig, g_shrink2);
        g_shrink2 = g_orig;

	BFSVisitorData visdata(&g_shrink2, visdata_orig.root);
        breadth_first_search(g_shrink2, *vertices(g_shrink2).first, boost::visitor(BFSVisitor(visdata)));
	uint r = visdata_orig.num_levels-1; // spanning tree radius
	//assert(visdata.num_levels == visdata_orig.num_levels);
	/* Let G be any planar graph with nonnegative vertex costs summing to no more than one.
	Suppose G has a spanning tree of radius r.
	Then the vertices of G can be partitioned into three sets A, B, C, such that no edge joins a vertex A with a vertex in B, neither A nor B has a total cost exceeding 2/3, and C contains no more than 2r+1 vertices, one the root of the tree. */

        // if g contains only two vertices then return trivially
        if( n <= 2 ){ 
                VertIter vit, vjt;
                tie(vit, vjt) = vertices(g_shrink2); 
                Partition p;
                p.a.insert(*vit);
                if( ++vit != vjt ) p.b.insert(*vit);
                return p; 
        }

        make_max_planar(g_shrink2);
        EmbedStruct em(&g_shrink2);

        uint cyclelengthlimit = find(STLALL(cycle), visdata.root) != cycle.end() ? 2*r+1 : 2*r-1;
        assert(cycle.size() <= cyclelengthlimit);

	/* Proof.  Assume no vertex has cost exceeding 1/3; otherwise the lemma is true.
	Embed G in the plane.  Make each face a triangle by adding a suitable number of additional edges.
	Any nontree edge (including each of the added edges) forms a simple cycle with some of the tree edges.
	This cycle is of length at most 2r+1 if it contains the root of the tree, at most 2r-1 otherwise.
	The cycle divides the plane (and the graph) into two parts, the inside and the outside of the cycle.
	We claim that at least one such cycle separates the graph so that neither the inside nor the outside contains vertices whose total cost exceeds 2/3.  This proves the lemma. */
        
        vector<edge_t> nontree_edges;
        for( auto[ei, ei_end] = edges(g_shrink2); ei != ei_end; ++ei ){
                if( source(*ei, g_shrink2) == target(*ei, g_shrink2) ) continue; // ignore workaround ciruclar nodes
                if( !visdata.is_tree_edge(*ei) ) nontree_edges.push_back(*ei);
        }

        vector<CycleCost> ccs;
        for( edge_t e : nontree_edges ){
                vector<vertex_t> cycle = visdata.get_cycle(source(e, g_shrink2), target(e, g_shrink2));
                CycleCost cc = compute_cycle_cost(cycle, g_shrink2, visdata, em);
                ccs.push_back(cc);
        }

        uint mindiff = UINT_MAX;
        uint min_index = UINT_MAX;
        for( uint i = 0; i < nontree_edges.size(); ++i ){
                uint costdiff = abs((int)ccs[i].inside - (int)ccs[i].outside);
                if( costdiff < mindiff ){
                        mindiff = costdiff;
                        min_index = i;
                }
        }
	
        edge_t xz = nontree_edges[min_index];
        cout << "xz: " << source(xz, g_shrink2) << ", " << target(xz, g_shrink2) << '\n';

        auto cycle2 = visdata.get_cycle(xz);

        auto p = Partition(cycle2, g_shrink2, em);
        // Let A be all verts inside the cycle
        // Let B be all verts outside the cycle 
        // Let C be all verts on the cycle

	/*Proof of claim.
	Let (x,z) be the nontree edge whose cycle minimizes the maximum cost either inside or outside the cycle.  Break ties by choosing the nontree edge whose cycle has the smallest number of faces on the same side as the maximum cost.  If ties remain, choose arbitrarily.

	Suppose without loss of generality that the graph is embedded so that the cost inside the cycle (x z) cycle is at least as great as the cost outside the cycle.  If the vertices inside the cycle have total cost not exceeding 2/3, the claim is true.  Suppose the vertices inside the cycle have total cost exceeding 2/3.  We show by case analysis that this contradicts the choice of (x, z).
	Consider the face which has (x, z) as a boundary edge and lies inside the cycle.
	This face is a triangle; let y be its third vertex.
	The properties of (x, y) and (y, z) determine which of the following case applies.
	Figure 4 illustrates the cases.

	1) Both (x, y) and (y, z) lie on the cycle.  Then the face (x, y, z) is the cycle, which is impossible since vertices lie inside the cycle.
	2) One of (x, y) and (y, z) (say (x, y)) lies on the cycle. Then (y, z) is a nontree edge defining a cycle which contains within it the same vertices as the original cycle but one less face. This contradicts the choice of (x, z).
	3) Neither (x, y) nor (y, z) lies on the cycle.
		a) Both (x, y) and (y, z) are tree edges.  This is impossible since the tree itself contains no cycles.
		b) One of (x, y) and (y, z) (say x, y) is a tree edge.  Then (y, z) is a nontree edge defining a cycle which contains one less vertex (namely y) within it than the original cycle.
	The inside of the (y, z) cycle contains no more cost and one less face than the inside of the (x, z) cycle.  Thus if the cost inside the (y, z) cycle is greater than the cost outside the cycle, (y, z)
	would have been chosen in place of (x, z).
	On the other hand, suppose the cost inside the (y, z) cycle is no greater than the cost outside.
	The cost outside the (y, z) cycle is equal to the cost outside the (x, z) cycle plus the cost of y.
	Since both the cost outside the (x, z) cycle and the cost of y are less than 1/3,
	the cost outside the (y, z) cycle is less than 2/3, and (y, z) would have been chosen in place of (x, z).
		c) Neither (x, y) nor (y, z) is a tree edge. Then each of (x, y) and (y, z) defines a cycle, and every vertex inside the (x, z) cycle is either inside the (x, y) cycle, inside the (y, z) cycle, or on the boundary of both.
		Of the (x, y) and (y, z) cycles, choose the one (say (x, y)) which has inside it more total cost. The (x, y) cycle has no more cost and strictly fewer faces inside it than the (x, z) cycle.
		Thus if the cost inside the (x, y) cycle is greater than the cost outside, (x, y) would have been chosen in place of (x, z).
	On the other hand, suppose the cost inside the (x, y) cycle is no greater than the cost outside.
	Since the inside of the (x, z) cycle has cost exceeding 2/3, the (x, y) cycle and its inside together have cost exceeding 1/3, and the outside of the (x, y) cycle has cost less than 2/3.
	Thus (x, y) would have been chosen in place of (x, z).
	Thus all cases are impossible, and the (x, z) cycle satisfies the claim. */

        p.verify_sizes_lemma2(r, visdata.root);

        return p;
}

// called when the middle part exceeds 2/3
Partition lemma3_exceeds23(Graph& g_shrink2, BFSVisitorData const& vis_data_shrunken, uint l1, uint l2, vector<vertex_t> const& cycle)
{
        cout << "g_shrink2:\n";
        print_graph(g_shrink2);
        print_graph_addresses(g_shrink2);
        /*Graph g_shrink2(g_orig);
        copy_graph(g_orig, g_shrink2);
        g_shrink2 = g_orig;*/

        cout << "middle partition has cost exceeding 2/3\n";

        //delete all verts on level l2 and above 
        VertIter vit, vjt;
        tie(vit, vjt) = vertices(g_shrink2); 
        for( VertIter next = vit; vit != vjt; vit = next ){
                ++next;
                if( !vis_data_shrunken.verts.contains(*vit) ){
                        cout << "ignoring vertex: " << *vit << '\n';
                }
                if( vis_data_shrunken.verts.find(*vit)->second.level >= l2 ){
                        //cout << "killing vertex " << vmap_shrunk.vert2uint[*vit] << " of level l2 or above: " << vis_data_orig.verts.find(*vit)->second.level << " >= " << l[2] << '\n';
                        kill_vertex(*vit, g_shrink2);
                }
        }

        //shrink all verts on levels l1 and below to a single vertex of cost zero

        // find a vertex to call cost zero
        vertex_t costzero = Graph::null_vertex(); 
        tie(vit, vjt) = vertices(g_shrink2); 
        for( VertIter next = vit; vit != vjt; vit = next ){
                ++next;
                if( !vis_data_shrunken.verts.contains(*vit) ){
                        cout << "ignoring vertex: " << *vit << '\n';
                        continue;
                }

                uint level = vis_data_shrunken.verts.find(*vit)->second.level;
                if( level <= l1 ){
                        costzero = *vit;
                        break;
                }
        }

        assert(costzero != Graph::null_vertex());

        // contract
        tie(vit, vjt) = vertices(g_shrink2); 
        for( VertIter next = vit; vit != vjt; vit = next ){
                ++next;
                if( !vis_data_shrunken.verts.contains(*vit) ){
                        cout << "ignoring vertex: " << *vit << '\n';
                }

                uint level = vis_data_shrunken.verts.find(*vit)->second.level;
                if( level <= l1 && *vit != costzero ){
                        costzero = contract_vertices(costzero, *vit, g_shrink2);
                }
        }

        //The new graph has a spanning tree radius of l2 - l1 -1 whose root corresponds to vertices on levels l1 and below in the original graph
        uint r = l2 - l1 - 1;
	assert(r == vis_data_shrunken.num_levels-1);
        //Apply Lemma 2 to the new graph, A* B* C*
        Partition star_p = lemma2(g_shrink2, cycle, vis_data_shrunken);
        vertex_t star_root;

        Partition p;
        p.a = star_p.a.size() > star_p.b.size() ? star_p.a : star_p.b; 

        p.c = star_p.c; 
        p.c.erase(star_root);
        // add verts on levels l1 and l2 in the original graph 
        tie(vit, vjt) = vertices(g_shrink2); 
        for( VertIter next = vit; vit != vjt; vit = next ){
                ++next;
                assert(vis_data_shrunken.verts.contains(*vit));
                if( vis_data_shrunken.verts.find(*vit)->second.level == l2 ||
                        vis_data_shrunken.verts.find(*vit)->second.level == l1 ){
                                p.c.insert(*vit);
                }
        } 

        // all verts not already in A or C go in B
        tie(vit, vjt) = vertices(g_shrink2); 
        for( VertIter next = vit; vit != vjt; vit = next ){
                ++next;
                if( p.a.find(*vit) == p.a.end() &&
                        p.c.find(*vit) == p.c.end() ){
                                p.b.insert(*vit);

                        }
        }

        return p; 
        /* By Lemma 2, A has total cost <= 2/3
        But A and C* have total cost >= 1/3, so B also has total cost <= 2/3
        Futhermore, C contains no more than L[l1] + L[l2] + 2(l2 - l1 - 1) */
}

Partition lemma3_l1greaterequall2(GraphCR g_shrink2, BFSVisitorData const& vis_data_orig, uint l1, uint r)
{
        //cout << "l1 is greater than or equal to l2\n"; 

        Partition p;
        for( auto[vei, vend] = vertices(g_shrink2); vei != vend; ++vei ){ 
                vertex_t v = *vei;
                if( ! vis_data_orig.verts.contains(v) ){
			cout << "ignoring non bfs vertex: " << v << '\n';
			continue;
		}
                uint level = vis_data_orig.verts.find(v)->second.level;

                //cout << "level of " << ii << ": " << vis_data_orig.verts.find(v)->second.level << "  ";
                if( level <  l1 ){                 cout << " belongs to first part\n";  p.a.insert(v); continue; }
                if( level >= l1+1 && level <= r ){ cout << " belongs to middle part\n"; p.b.insert(v); continue; }
                if( level == l1 ){                 cout << " belongs to last part\n";   p.c.insert(v); continue; }
                assert(0);
        } 
        /*cout << "A = all verts on levels 0    thru l1-1\n";
        cout << "B = all verts on levels l1+1 thru r\n";
        cout << "C = all verts on level l1\n";*/
        return p;
}

Partition lemma3_lessequal23(set<vertex_t> const& first_part, set<vertex_t> const& middle_part, set<vertex_t> const& last_part, set<vertex_t>& deleted_part, Graph const* g)
{
        cout << "middle partition has cost <= 2/3\n";

        Partition p;
        p.a = first_part;
        p.b = middle_part;
        p.c = last_part;

        set<vertex_t> const* costly_part;
        set<vertex_t> const* other1;
        set<vertex_t> const* other2;
        p.get_most_costly_part(&costly_part, &other1, &other2);

        p.print(g);
        cout <<   "A = most costly part of the 3 : "; for( auto& a : *costly_part ) cout << a << ' ';
        cout << "\nB = remaining 2 parts         : "; for( auto& b : *other1      ) cout << b << ' '; for( auto& b : *other2 ) cout << b << ' '; 
        cout << "\nC = deleted verts on l1 and l2: "; for( auto& v : deleted_part ) cout << v << ' '; cout << '\n';

        Partition p2;
        p2.a = *costly_part;
        p2.c = deleted_part;
        p2.b = *other1;
        p2.b.insert(other2->begin(), other2->end());
        p2.print(g);
        return p2; 
}

/* Let G be any n-vertex connected planar graph having nonegative vertex consts summing to no more than one.
Suppose that the vertices of G are partitioned into levels according to their distance from some vertex v, and that L(l) denotes the number of vertices on level l.
If r is the maximum distance of any vertex from v, let r+1 be an additional level containing no vertices.
Given any two levels l1 and l2 such that levels 0 through l1-1 have total cost not exceeding 2/3 and levels l2+1 through r+1 have total cost not exceeding 2/3,
it is possible to find a partition A, B, C of the vertices of G such that no edge joins a vertex in A with a vertex in B, neither A nor B has total cost exceeding 2/3, and C contains no more than L(l1)+L(l2)+max{0,2(l2-l1-1)} vertices. */
Partition lemma3(GraphCR g_orig, vector<uint> const& L, uint l1, uint l2, uint r, BFSVisitorData const& vis_data_orig, BFSVisitorData const& vis_data_shrunken, vector<vertex_t> const& cycle, Graph* g_shrunk)
{
        uint n = vis_data_orig.verts.size();
        uint n_orig = num_vertices(g_orig); 
        cout << "n: " << n << '\n';
        cout << "n_orig: " << n_orig << '\n';

        if( n != n_orig ){
                // more than one connected component, we need to recompute l1 and l2 

                uint total = 0;
                uint level = 0;
                while( total < 2*n_orig/3 ) total += L[level++];

                total = 0;
                level = r+1;
                while( total < 2*n_orig/3 ){ 
                        total += L[level];
			if( 0 == level ) break;
			--level;
                }

                l2 = level;
        }

        uint cost_0thrul1m1 = 0;
        uint cost_l2p1thrur1 = 0;
        for( uint i = 0; i < r; ++i ){ 
                if( i+1 < l1 ) cost_0thrul1m1 += L[i];
                if( i >= l2+1 ) cost_l2p1thrur1 += L[i];
        }
        uint costlimit = 2*n/3;
        assert(cost_0thrul1m1 <= costlimit);
        assert(cost_l2p1thrur1 <= costlimit);


        //assert(vis_data_shrunken.assert_data());
        //assert(vis_data_orig.assert_data()); 
        //assert(n == n_orig);

        //uint total_cost = 0;

        auto prop_map = get(vertex_index, g_orig); // writing to this property map has side effects in the graph

        Partition p;
        if( l1 >= l2 ){
                p = lemma3_l1greaterequall2(g_orig, vis_data_orig, l1, r);
        } else {
                cout << "l1 < l2\n";

                set<vertex_t> first_part, middle_part, last_part, deleted_part;
                VertIter vei, vend;
                for( tie(vei, vend) = vertices(g_orig); vei != vend; ++vei ){ 
                        vertex_t v = *vei;
                        if( !vis_data_orig.verts.contains(v) ){
                                cout << "lemmas.cpp: ignoring bad vertex: " << v << " prop_map: " << prop_map[v] << '\n';
                                continue; 
                        }

                        uint level = vis_data_orig.verts.find(v)->second.level;

                        cout << "level of " << v << " prop_map " << prop_map[v] << " is : " << level << ", ";
                        fflush(stdout);
                        if( level == l1 || level == l2 ){     cout << v << " is deleted\n";                 deleted_part.insert(v); continue;}
                        if( level <  l1 ){                    cout << v << " belongs to first part (A)\n";  first_part.insert(v);   continue;}
                        if( level >= l1+1 && level <= l2-1 ){ cout << v << " belongs to middle part (B)\n"; middle_part.insert(v);  continue;}
                        if( level >  l2   ){                  cout << v << " belongs to last part (C)\n";   last_part.insert(v);    continue;}
                        assert(0);
                }

                uint ptotal = first_part.size() + middle_part.size() + last_part.size() + deleted_part.size();
                cout << "ptotal: " << ptotal << '\n';
                assert(ptotal == n);

                //the only part which can have cost > 2/3 is the middle part (B)
                cout << "first part size: " << first_part.size() << '\n';
                cout << "middle part size: " << middle_part.size() << '\n';
                cout << "third part size: " << last_part.size() << '\n';
                assert(first_part.size() <= 2*n/3);
                assert(last_part.size() <= 2*n/3);
                //p.print();

                p = middle_part.size() <= 2*n/3                                          ?
                    lemma3_lessequal23(first_part, middle_part, last_part, deleted_part, &g_orig) :
                    lemma3_exceeds23(*g_shrunk, vis_data_shrunken, l1, l2, cycle);
        }

        assert(p.verify_edges(g_orig));
	assert(p.verify_sizes_lemma3(L, l1, l2));
        return p;
}