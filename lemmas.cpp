#include "lemmas.h"
#include "graphutil.h"
#include <boost/graph/copy.hpp>
using namespace std;
using namespace boost;

/* Let G be any planar graph with nonnegative vertex costs summing to no more than one.
Suppose G has a spanning tree of radius r.
Then the vertices of G can be partitioned into three sets A, B, C such that no edge joins a vertex in A with a vertex in B, neither A nor B has total cost exceeding 2/3,
and C contains no more than 2r+1 vertices, one the root of the tree */
// r is spanning tree radius
Partition lemma2_c2r1(GraphCR g_orig, uint r, vector<vertex_t> const& cycle)
{
        uint n = num_vertices(g_orig);

        Graph g_shrink2;
        copy_graph(g_orig, g_shrink2);
        g_shrink2 = g_orig;

	//uint r = 0; // spanning tree radius

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

        Partition p;

        make_max_planar(g_shrink2);

	/* Proof.  Assume no vertex has cost exceeding 1/3; otherwise the lemma is true.
	Embed G in the plane.  Make each face a triangle by adding a suitable number of additional edges.
	Any nontree edge (including each of the added edges) forms a simple cycle with some of the tree edges.
	This cycle is of length at most 2r+1 if it contains the root of the tree, at most 2r-1 otherwise.
	The cycle divides the plane (and the graph) into two parts, the inside and the outside of the cycle.
	We claim that at least one such cycle separates the graph so that neither the inside nor the outside contains vertices whose total cost exceeds 2/3.  This proves the lemma. */
	
	/*Proof of claim.
	Let (x,z) be the nontree edge whose cycle minimizes the maximum cost either inside or outside the cycle.  Break ties by choosing the nontree edge whose cycle has the smallest number of faces on the same side as the maximum cost.  If ties remain, choose arbitrarily.

	Suppose without loss of generality that the graph is embedded so that the cost inside the cycle (x z) cycle is at least as great as the cost outside the cycle.  If the vertices inside the cycle have total cost not exceeding 2/3, the claim is true.  Suppose the vertices inside the cycle have total cost exceeding 2/3.  We show by case analysis that this contradicts the choice of (x, z).
	Consider the face which has (x, z) as a boundary edge and lies inside the cycle.
	This face is a triangle; let y be its third vertex.
	The properties of (x, y) and (y, z) determine which of the following case applies.
	Figure 4 illustrates the cases.


	1) Both (x, y) and (y, z) lie on the cycle.  Then the face (x, y, z) is the cycle,
	which is impossible since vertices lie inside the cycle.
	2) One of (x, y) and (y, z) (say (x, y)) lies on the cycle.
	Then (y, z) is a nontree edge defining a cycle which contains within it the same vertices as the original cycle but one less face.
	This contradicts the choice of (x, z).
	3) Neither (x, y) nor (y, z) lies on the cycle.
		a) Both (x, y) and (y, z) are tree edges.  This is impossible since the tree itself contains no cycles.
		b) One of (x, y) and (y, z) (say x, y) is a tree edge.  Then (y, z) is a nontree edge defining a cycle which contains one less vertex (namely y) within it than the original cycle.
	The inside of the (y, z) cycle contains no more cost and one less face than the inside of the (x, z) cycle.  Thus if the cost inside the (y, z) cycle is greater than the cost outside the cycle, (y, z)
	would have been chosen in place of (x, z).
	On the other hand, suppose the cost inside the (y, z) cycle is no greater than the cost outside.
	The cost outside the (y, z) cycle is equal to the cost outside the (x, z) cycle plus the cost of y.
	Since both the cost outside the (x, z) cycle and the cost of y are less than 1/3,
	the cost outside the (y, z) cycle is less than 2/3, and (y, z) would have been chosen in place of (x, z).
		c) Neither (x, y) nor (y, z) is a tree edge.
		Then each of (x, y) and (y, z) defines a cycle,
		and every vertex inside the (x, z) cycle is either inside the (x, y) cycle,
		inside the (y, z) cycle, or on the boundary of both.
		Of the (x, y) and (y, z) cycles, choose the one (say (x, y)) which has inside it
		more total cost.
		The (x, y) cycle has no more cost and strictly fewer faces inside it than the (x, z) cycle.
		Thus if the cost inside the (x, y) cycle is greater than the cost outside, (x, y) would have been chosen in place of (x, z).
	On the other hand, suppose the cost inside the (x, y) cycle is no greater than the cost outside.
	Since the inside of the (x, z) cycle has cost exceeding 2/3, the (x, y) cycle and its inside together have cost exceeding 1/3, and the outside of the (x, y) cycle has cost less than 2/3.
	Thus (x, y) would have been chosen in place of (x, z).
	Thus all cases are impossible, and the (x, z) cycle satisfies the claim. */
        return p;
}

// called when the middle part exceeds 2/3
Partition lemma3_exceeds23(GraphCR g_orig, BFSVisitorData const& vis_data_orig, uint l1, uint l2, vector<vertex_t> const& cycle)
{
        Graph g_shrink2(g_orig);
        copy_graph(g_orig, g_shrink2);
        g_shrink2 = g_orig;

        cout << "middle partition has cost exceeding 2/3\n";

        //delete all verts on level l2 and above 
        VertIter vit, vjt;
        tie(vit, vjt) = vertices(g_shrink2); 
        for( VertIter next = vit; vit != vjt; vit = next ){
                ++next;
                assert(vis_data_orig.verts.contains(*vit));
                if( vis_data_orig.verts.find(*vit)->second.level >= l2 ){
                        //cout << "killing vertex " << vmap_shrunk.vert2uint[*vit] << " of level l2 or above: " << vis_data_orig.verts.find(*vit)->second.level << " >= " << l[2] << '\n';
                        kill_vertex(*vit, g_shrink2);
                }
        }

        //shrink all verts on levels l1 and below to a single vertex of cost zero
        VertIter v_cost_zero;
        tie(v_cost_zero, vjt) = vertices(g_shrink2);
        vit = ++v_cost_zero;

        while( vit != vjt ){
                contract_vertices(*v_cost_zero, *vit, g_shrink2);
                ++vit;
        } 

        //The new graph has a spanning tree radius of l2 - l1 -1 whose root corresponds to vertices on levels l1 and below in the original graph
        uint r = l2 - l1 - 1;
        //Apply Lemma 2 to the new graph, A* B* C*
        Partition star_p = lemma2_c2r1(g_shrink2, r, cycle);
        vertex_t star_root;

        Partition p;
        p.a = star_p.a.size() > star_p.b.size() ? star_p.a : star_p.b; 

        p.c = star_p.c; 
        p.c.erase(star_root);
        // add verts on levels l1 and l2 in the original graph 
        tie(vit, vjt) = vertices(g_shrink2); 
        for( VertIter next = vit; vit != vjt; vit = next ){
                ++next;
                assert(vis_data_orig.verts.contains(*vit));
                if( vis_data_orig.verts.find(*vit)->second.level == l2 ||
                        vis_data_orig.verts.find(*vit)->second.level == l1 ){
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
        But A U C* has total cost >= 1/3, so B also has total cost <= 2/3
        Futhermore, C contains no more than L[l1] + L[l2] + 2(l2 - l1 - 1) */
}

Partition lemma3_l1greaterequall2(GraphCR g_shrink2, BFSVisitorData const& vis_data_orig, uint l1, uint r)
{
        //cout << "l1 is greater than or equal to l2\n"; 

        Partition p;
        VertIter vei, vend;
        for( tie(vei, vend) = vertices(g_shrink2); vei != vend; ++vei ){ 
                vertex_t v = *vei;
                assert(vis_data_orig.verts.contains(v));
                //cout << "level of " << ii << ": " << vis_data_orig.verts.find(v)->second.level << "  ";
                if( vis_data_orig.verts.find(v)->second.level <  l1 )                                       { cout << " belongs to first part\n";  p.a.insert(v); continue; }
                if( vis_data_orig.verts.find(v)->second.level >= l1+1 && vis_data_orig.verts.find(v)->second.level <= r ){ cout << " belongs to middle part\n"; p.b.insert(v); continue; }
                if( vis_data_orig.verts.find(v)->second.level == l1 )                                       { cout << " belongs to last part\n";   p.c.insert(v); continue; }
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
Partition lemma3(GraphCR g_orig, vector<uint> const& L, uint l1, uint l2, uint r, BFSVisitorData const& vis_data_orig, BFSVisitorData const& vis_data_shrunken, vector<vertex_t> const& cycle)
{
        //assert(vis_data_shrunken.assert_data());
        //assert(vis_data_orig.assert_data()); 
        uint n = vis_data_orig.verts.size();
        //uint n = num_vertices(g_orig); 
        cout << "n: " << n << '\n';

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
                //p.print();

                p = middle_part.size() <= 2*n/3                                          ?
                    lemma3_lessequal23(first_part, middle_part, last_part, deleted_part, &g_orig) :
                    lemma3_exceeds23(g_orig, vis_data_orig, l1, l2, cycle);
        }

        assert(p.verify_edges(g_orig));
	assert(p.verify_sizes_lemma3(L, l1, l2));
        return p;
}
