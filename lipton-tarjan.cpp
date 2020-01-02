#include "lipton-tarjan.h"
#include "theorem4.h"
#include "lemmas.h"
#include "strutil.h"
#include "BFSVert.h"
#include "BFSVisitorData.h"
#include "BFSVisitor.h"
#include "EmbedStruct.h"
#include "ScanVisitor.h"
#include "graphutil.h"
#include <boost/lexical_cast.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/planar_canonical_ordering.hpp>
#include <boost/graph/is_straight_line_drawing.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp> 
#include <boost/graph/make_biconnected_planar.hpp>
#include <boost/graph/make_maximal_planar.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/copy.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/pending/indirect_cmp.hpp>
#include <boost/range/irange.hpp> 
#include <boost/bimap.hpp>
#include <boost/config.hpp>
#include <iostream>
#include <algorithm>
#include <utility>
#include <csignal>
#include <iterator>
#include <cassert>
using namespace std;
using namespace boost; 

// Step 10: construct_vertex_partition
// Time:    O(n)
//
// Use the cycle found in Step 9 and the levels found in Step 4 (l1_and_k) to construct a satisfactory vertex partition as described in the proof of Lemma 3
// Extend this partition from the connected component chosen in Step 2 to the entire graph as described in the proof of Theorem 4.
Partition construct_vertex_partition(GraphCR g_orig, uint l[3], BFSVisitorData const& vis_data_orig, BFSVisitorData const& vis_data, vector<vertex_t> const& cycle)
{
        uint n_orig = num_vertices(g_orig);
        //cout << "n_orig: " << n_orig << '\n';

        //cout  << "\n------------ 10  - Construct Vertex Partition --------------\n";
        //cout << "g_orig:\n";
        //print_graph(g_orig);
	//print_graph_special(g_orig, vmap);
        /*cout << "l0: " << l[0] << '\n';
        cout << "l1: " << l[1] << '\n';
        cout << "l2: " << l[2] << '\n';*/

        uint r = vis_data.num_levels;
        //cout << "r max distance: " << r << '\n';

        return lemma3_cllmax(g_orig, l, r, vis_data_orig, vis_data, cycle);
}

// Step 9: Improve Separator
// Time:   O(n)
//
// Let (vi, wi) be the nontree edge whose cycle is the current candidate to complete the separator.
// If the cost inside the cycle exceeds 2/3, find a better cycle by the following method.
// 	Locate the triangle (vi, y, wi) which has (vi, wi) as a boundary edge and lies inside the (vi, wi) cycle.
//      If either (vi, y) or (y, wi) is a tree edge, let (vi+1, wi+1) be the nontree edge among (vi, y) and (y, wi).
//      Compute the cost inside the (vi+1, wi+1) cycle from the cost inside the (vi, wi) cycle and the cost of vi, y and wi.
// 	If neither (vi, y) nor (y, wi) is a tree edge, determine the tree path from y to the (vi, wi) cycle by following parent pointers from y.
//      Let z be the vertex on the (vi, wi) cycle reached during this search.  Compute the total cost of all vertices except z on this tree path.
//      Scan the tree edges inside the (y, wi) cycle, alternately scanning an edge in one cycle and an edge in the other cycle.
//      Stop scanning when all edges inside one of the cycles have been scanned.  Compute the cost inside this cycle by summing the associated costs of all scanned edges.
//      Use this cost, the cost inside the (vi, wi) cycle, and the cost on the tree path from y to z to compute the cost inside the other cycle.
//      Let (vi+1, wi+1) be the edge among (vi, y) and (y, wi) whose cycle has more cost inside it.
// 	Repeat Step 9 until finding a cycle whose inside has cost not exceeding 2/3.
Partition improve_separator(GraphCR g_orig, Graph& g_shrunk, CycleCost& cc, edge_t completer_candidate_edge, BFSVisitorData const& vis_data_orig,
                                BFSVisitorData const& vis_data, vector<vertex_t> cycle, EmbedStruct const& em, bool cost_swapped, uint l[3])
{
        //cout << "---------------------------- 9 - Improve Separator -----------\n";
        //print_graph(g_shrunk);
        //print_edges(g_shrunk, vmap_shrunk);

        /*cout << "cycle: ";
        for( uint i = 0; i < cycle.size(); ++i ) cout << vmap_shrunk.vert2uint[cycle[i]] << ' ';
        cout << '\n';*/

        uint n_orig = num_vertices(g_orig);
        uint n = num_vertices(g_shrunk);
        //cout << "n_orig: " << n_orig << ", n: " << n << '\n';

        while( cc.inside > 2.*n/3 ){
                /*cout << "nontree completer candidate edge: " << to_string(completer_candidate_edge, vmap_shrunk, g_shrunk) << '\n';
                cout << "cost inside: " << cc.inside  << '\n';
                cout << "cost outide: " << cc.outside << '\n';
                cout << "looking for a better cycle\n";*/

		// Let (vi, wi) be the nontree edge whose cycle is the current candidate to complete the separator
                vertex_t vi = source(completer_candidate_edge, g_shrunk);
                vertex_t wi = target(completer_candidate_edge, g_shrunk); 
                assert(!vis_data.is_tree_edge(completer_candidate_edge));
                //cout << "   vi: " << vmap_shrunk.vert2uint[vi] << '\n';
                //cout << "   wi: " << vmap_shrunk.vert2uint[wi] << '\n';

                set<vertex_t> neighbors_of_v = get_neighbors(vi, g_shrunk);
                set<vertex_t> neighbors_of_w = get_neighbors(wi, g_shrunk); 
                pair<vertex_t, vertex_t> neighbors_vw = get_intersection(neighbors_of_v, neighbors_of_w);
                //cout << "   neighbors_vw_begin : " << vmap_shrunk.vert2uint[neighbors_vw.first] << '\n';
                //cout << "   neighbors_vw_rbegin: " << vmap_shrunk.vert2uint[neighbors_vw.second] << '\n';

		// Locate the triangle (vi, y, wi) which has (vi, wi) as a boundary edge and lies inside the (vi, wi) cycle.  
		// one of the two vertices in the set neighbors_vw is y.  Maybe it's .begin(), so we use is_edge_inside_outside_or_on_cycle to test if it is.
                pair<edge_t, bool> maybe_y = edge(vi, neighbors_vw.first, g_shrunk);
                assert(maybe_y.second); // I'm assuming the bool means that the edge_t exists?  Boost Graph docs don't say
                //cout << "maybe_y: " << to_string(maybe_y.first, vmap_shrunk, g_shrunk) << '\n';

                vertex_t common_vert_on_cycle = find(STLALL(cycle), neighbors_vw.first) == cycle.end() ?
                                                neighbors_vw.second :
                                                neighbors_vw.first;
                //cout << "common vert on cycle: " << vmap_shrunk.vert2uint[common_vert_on_cycle] << '\n';
                assert(find(STLALL(cycle), common_vert_on_cycle) != cycle.end());

                InsideOutOn insideout = is_edge_inside_outside_or_on_cycle(maybe_y.first, common_vert_on_cycle, cycle, g_shrunk, em.em);
		assert(insideout != ON);
                vertex_t y = (insideout == INSIDE) ? neighbors_vw.first : neighbors_vw.second; // We now have the (vi, y, wi) triangle

                //cout << "   y: " << vmap_shrunk.vert2uint[y] << '\n';
                pair<edge_t, bool> viy_e = edge(vi, y, g_shrunk); assert(viy_e.second); edge_t viy = viy_e.first;
                pair<edge_t, bool> ywi_e = edge(y, wi, g_shrunk); assert(ywi_e.second); edge_t ywi = ywi_e.first; 
                edge_t next_edge;

		// if either (vi, y) or (y, wi) is a tree edge, 
                if ( vis_data.is_tree_edge(viy) || vis_data.is_tree_edge(ywi) ){
			// determine the tree path from y to the (vi, wi) cycle by following parent pointers from y.
                        //cout << "   at least one tree edge\n";
                        next_edge = vis_data.is_tree_edge(viy) ? ywi : viy;
                        assert(!vis_data.is_tree_edge(next_edge));

                        // Compute the cost inside the (vi+1 wi+1) cycle from the cost inside the (vi, wi) cycle and the cost of vi, y, and wi.  See Fig 4.
                        uint cost[4] = {vis_data.verts.find(vi)->second.descendant_cost,
					vis_data.verts.find(y )->second.descendant_cost,
					vis_data.verts.find(wi)->second.descendant_cost,
					cc.inside};
                        vector<vertex_t> new_cycle = get_cycle(source(next_edge, g_shrunk), target(next_edge, g_shrunk), vis_data);
                        cc = compute_cycle_cost(new_cycle, g_shrunk, vis_data, em); // !! CHEATED !!
                        if( cost_swapped ) swap(cc.outside, cc.inside);
                } else {
                        // Determine the tree path from y to the (vi, wi) cycle by following parents of y.
                        //cout << "   neither are tree edges\n";
                        vector<vertex_t> y_parents = ancestors(y, vis_data);
                        uint i = 0;
                        while( !on_cycle(y_parents[i++], cycle, g_shrunk) );

                        // Let z be the vertex on the (vi, wi) cycle reached during the search.
                        vertex_t z = y_parents[i++];
                        //cout << "    z: " << vmap_shrunk.vert2uint[z] << '\n';
                        y_parents.erase(y_parents.begin()+i, y_parents.end());
                        assert(y_parents.size() == i);

                        // Compute the total cost af all vertices except z on this tree path.
                        uint path_cost = y_parents.size() - 1;
                        //cout << "    y-to-z-minus-z cost: " << path_cost << '\n';

                        // Scan the tree edges inside the (y, wi) cycle, alternately scanning an edge in one cycle and an edge in the other cycle.
                        // Stop scanning when all edges inside one of the cycles have been scanned.  Compute the cost inside this cycle by summing the associated costs of all scanned edges.
                        // Use this cost, the cost inside the (vi, wi) cycle, and the cost on the tree path from y to z to compute the cost inside the other cycle.
                        vector<vertex_t> cycle1 = get_cycle(vi, y, vis_data);
                        vector<vertex_t> cycle2 = get_cycle(y, wi, vis_data);
                        CycleCost cost1 = compute_cycle_cost(cycle1, g_shrunk, vis_data, em);
                        CycleCost cost2 = compute_cycle_cost(cycle2, g_shrunk, vis_data, em);
                        if( cost_swapped ){
                                swap(cost1.inside, cost1.outside);
                                swap(cost2.inside, cost2.outside);
                        }

                        // Let (vi+1, wi+1) be the edge among (vi, y) and (i, wi) whose cycle has more cost inside it.
                        if( cost1.inside > cost2.inside ){ next_edge = edge(vi, y, g_shrunk).first; cc = cost1; cycle = cycle1;}
                        else                             { next_edge = edge(y, wi, g_shrunk).first; cc = cost2; cycle = cycle2;}
                } 
                completer_candidate_edge = next_edge;
        }
        //cout << "found cycle with inside cost " << cc.inside << " which is less than 2/3\n";
        //print_cycle(cycle);

	return construct_vertex_partition(g_orig, l, vis_data_orig, vis_data, cycle); // step 10
}


// Step 8: locate_cycle
// Time: O(n)
//
// Choose any nontree edge (v1, w1).
// Locate the corresponding cycle by following parent pointers from v1 and w1.
// Compute the cost on each side of this cycle by scanning the tree edges incident on either side of the cycle and summing their associated costs.
// If (v, w) is a tree edge with v on the cycle and w not on the cycle, the cost associated with (v,w) is the descendant cost of w if v is the parent of w,
// and the cost of all vertices minus the descendant cost of v if w is the parent of v.
// Determine which side of the cycle has greater cost and call it the "inside"
Partition locate_cycle(GraphCR g_orig, Graph& g_shrunk, BFSVisitorData const& vis_data_orig, BFSVisitorData const& vis_data, uint l[3])
{
        uint n = num_vertices(g_orig);
        //cout  << "----------------------- 8 - Locate Cycle -----------------\n"; 
        //print_graph(g_shrunk);
        edge_t completer_candidate_edge;
        
        try {
                completer_candidate_edge = arbitrary_nontree_edge(g_shrunk, vis_data);
        } catch (NoNontreeEdgeException const& e){
                vector<vertex_t> cycle;
                CycleCost cc;
                cc.inside = 0;
                cc.outside = n; // force Step 9 to exit without going through any iterations
                bool cost_swapped = false;
                EmbedStruct em(&g_shrunk);
                return improve_separator(g_orig, g_shrunk, cc, completer_candidate_edge, vis_data_orig, vis_data, cycle, em, cost_swapped, l); // step 9 
        }
        vertex_t v1 = source(completer_candidate_edge, g_shrunk);
        vertex_t w1 = target(completer_candidate_edge, g_shrunk); 
        //cout << "ancestors v1...\n";
        vector<vertex_t> parents_v   = ancestors(v1, vis_data);
        //cout << "ancestors v2...\n";
        vector<vertex_t> parents_w   = ancestors(w1, vis_data); 
        vertex_t common_ancestor    = get_common_ancestor(parents_v, parents_w);
        //cout << "common ancestor: " << common_ancestor << '\n'; 
        vector<vertex_t> cycle = get_cycle(v1, w1, common_ancestor, vis_data);

        EmbedStruct em(&g_shrunk);
        CycleCost cc = compute_cycle_cost(cycle, g_shrunk, vis_data, em); 
	bool cost_swapped;
        if( cc.outside > cc.inside ){
                swap(cc.outside, cc.inside);
                cost_swapped = true;
                //cout << "!!!!!! cost swapped !!!!!!!!\n";
        } else cost_swapped = false;
        //cout << "total inside cost:  " << cc.inside  << '\n'; 
        //cout << "total outside cost: " << cc.outside << '\n';

	return improve_separator(g_orig, g_shrunk, cc, completer_candidate_edge, vis_data_orig, vis_data, cycle, em, cost_swapped, l); // step 9
}

// Step 7: new_bfs_and_make_max_planar
// Time:   O(n)
// 
// Construct a breadth-first spanning tree rooted at x in the new (shrunken) graph.
// (This can be done by modifying the breadth-first spanning tree constructed in Step 3 bfs_and_levels.)
// Record, for each vertex v, the parent of v in the tree, and the total cost of all descendants of v includiing v itself.
// Make all faces of the new graph into triangles by scanning the boundary of each face and adding (nontree) edges as necessary.
Partition new_bfs_and_make_max_planar(GraphCR g_orig, Graph& g_shrunk, BFSVisitorData const& vis_data_orig, vertex_t x_gone, vertex_t x, uint l[3])
{
        cout  << "-------------------- 7 - New BFS and Make Max Planar -----\n";
        cout << "g_orig:\n";
        print_graph(g_orig);
        cout << "g_shrunk:\n";
        print_graph(g_shrunk);
        //reset_vertex_indices(g_shrunk);
        //reset_edge_index(g_shrunk);
        BFSVisitorData shrunken_vis_data(&g_shrunk, x);

        //vis_data.reset(&g_shrunk);
        shrunken_vis_data.root = (x_gone != Graph::null_vertex()) ? x_gone : x;
        ++shrunken_vis_data.verts[shrunken_vis_data.root].descendant_cost;

        //cout << "root: " << vmap_shrunk.vert2uint[shrunken_vis_data.root] << '\n'; 
        uint n = num_vertices(g_shrunk);
        cout << "n:    " << n << '\n'; 
        cout << "null vertex: " << Graph::null_vertex() << '\n';

        BFSVisitor visit = BFSVisitor(shrunken_vis_data);

        auto bvs = boost::visitor(visit);

        print_graph(g_shrunk);

        // workaround for https://github.com/boostorg/graph/issues/195 
        VertIter vit, vjt;
        tie(vit, vjt) = vertices(g_shrunk);
        for( VertIter next = vit; vit != vjt; ++vit){
                if( 0 == in_degree(*vit, g_shrunk) + out_degree(*vit, g_shrunk) ){
                        add_edge(*vit, *vit, g_shrunk);
                }
        }

        breadth_first_search(g_shrunk, shrunken_vis_data.root, bvs);

        make_max_planar(g_shrunk);
        //reset_vertex_indices(g_shrunk);
        reset_edge_index(g_shrunk);

        print_graph(g_shrunk);

	return locate_cycle(g_orig, g_shrunk, vis_data_orig, shrunken_vis_data, l);  // step 8
}

// Step 6: Shrinktree
// Time:   big-theta(n)
//
// Delete all vertices on level l2 and above.
// Construct a new vertex x to represent all vertices on levels 0 through l0.
// Construct a boolean table with one entry per vertex.
// Initialize to true the entry for each vertex on levels 0 through l0 and
// initialize to false the entry for each vertex on levels l0 + 1 through l2 - 1.
// The vertices on levels 0 through l0 correspond to a subtree of the breadth-first spanning tree
// generated in Step 3 (bfs_and_levels).
// Scan the edges incident to this tree clockwise around the tree.
// When scanning an edge(v, w) with v in the tree, check the table entry for w.
// If it is true, delete edge(v, w).
// If it is false, change it to true, construct an edge(x,w) and delete edge(v,w).
// The result of this step is a planar representation of the shrunken graph to which Lemma 2 is to be applied.

//const uint X_VERT_UINT = 999999;

vector<vertex_t> shrinktree_deletel2andabove(Graph& g, uint l[3], BFSVisitorData const& vis_data_orig)
{
        vector<vertex_t> replaced_verts;
	VertIter vit, vjt;
        tie(vit, vjt) = vertices(g); 
        for( VertIter next = vit; vit != vjt; vit = next ){
                ++next;
                if( !vis_data_orig.verts.contains(*vit) ) continue; // *vit is in a different connected component

                if( vis_data_orig.verts.find(*vit)->second.level >= l[2] ){
                        //cout << "killing vertex " << vmap_shrunk.vert2uint[*vit] << " of level l2 " << vis_data_orig.verts.find(*vit)->second.level << " >= " << l[2] << '\n';
                        kill_vertex(*vit, g); 
                }
                if( vis_data_orig.verts.find(*vit)->second.level <= l[0] ){
                        //cout << "going to replace vertex " << vmap_shrunk.vert2uint[*vit] << " of level l0 " << vis_data_orig.verts.find(*vit)->second.level << " <= " << l[0] << '\n';
                        replaced_verts.push_back(*vit);
                }
        }
        return replaced_verts;
}

Partition shrinktree(GraphCR g_orig, Graph& g_copy, BFSVisitorData const& vis_data_orig, uint l[3])
{
        Graph& g_shrunk = g_copy;
        cout << "---------------------------- 6 - Shrinktree -------------\n";
        print_graph(g_copy);
        cout << "n: " << num_vertices(g_shrunk) << '\n';

        // delete all vertices on level l2 and above
        vector<vertex_t> replaced_verts = shrinktree_deletel2andabove(g_shrunk, l, vis_data_orig);

        vertex_t x = add_vertex(g_shrunk);
        auto prop_map = get(vertex_index, g_shrunk);
        //prop_map[x] = X_VERT_UINT;

        //vmap_shrunk.uint2vert[vmap_shrunk.vert2uint[x] = X_VERT_UINT] = x; 
        map<vertex_t, bool> table; // x will not be in this table
	VertIter vit, vjt;
        for( tie(vit, vjt) = vertices(g_shrunk); vit != vjt; ++vit ){
                if( *vit == x ) continue; // the new x isn't in visdata, std::map::find() will fail
                if( !vis_data_orig.verts.contains(*vit) ) continue; // *vit may be in a different connected component
                uint level = vis_data_orig.verts.find(*vit)->second.level;
                table[*vit] = level <= l[0];
                //cout << "vertex " << vmap_shrunk.vert2uint[*vit] << " at level " << vis_data_orig.verts.find(*vit)->second.level << " is " << (table[*vit] ? "TRUE" : "FALSE") << '\n';
        }

        //reset_vertex_indices(g_shrunk);
        //reset_edge_index(g_shrunk);
        EmbedStruct em(&g_shrunk);
        assert(em.test_planar());

        ScanVisitor svis(&table, &g_shrunk, x, l[0]);
        svis.scan_nonsubtree_edges_clockwise(*vertices(g_shrunk).first, g_shrunk, em.em, vis_data_orig);
        svis.finish();

        vertex_t x_gone = Graph::null_vertex();
        if( !degree(x, g_shrunk) ){
                cout << "no edges to x found, deleting x\n";
                kill_vertex(x, g_shrunk);
                x_gone = *vertices(g_shrunk).first;
                x = Graph::null_vertex();
                cout << "x_gone: " << x_gone << '\n';
        } else {
                cout << "deleting all vertices x has replaced\n";
                for( vertex_t& v : replaced_verts ) {cout << "killing " << v << '\n'; kill_vertex(v, g_shrunk); }// delete all vertices x has replaced

                assert(vertex_exists(x, g_shrunk));

                uint n2 = num_vertices(g_shrunk);
                cout << "shrunken size: " << n2 << '\n';
        }

        cout << "x: " << prop_map[x] << '\n';

	return new_bfs_and_make_max_planar(g_orig, g_shrunk, vis_data_orig, x_gone, x, l); // step 7
}

// Step 5: find_more_levels
// Time:   O(n)
//
// Find the highest level l0 <= l1 such that L(l0) + 2(l1 - l0) <= 2*sqrt(k).
// Find the lowest level l2 >= l1 + 1 such that L(l2) + 2(l2-l1-1) <= 2*sqrt(n-k)
Partition find_more_levels(GraphCR g_orig, Graph& g_copy, uint k, uint l[3], vector<uint> const& L, BFSVisitorData const& vis_data_orig)
{
        //cout  << "---------------------------- 5 - Find More Levels -------\n";
        //print_graph(g_copy);
        float sq  = 2 * sqrt(k); 
        float snk = 2 * sqrt(num_vertices(g_copy) - k); 
        cout << "sq:     " << sq << '\n';
        cout << "snk:    " << snk << '\n';
        cout << "L size: " << L.size() << '\n';

        l[0] = l[1];
        //cout << "l[0]:   " << l[0] << '\n';
        while( l[0] < L.size() ){
                float val = L.at(l[0]) + 2*(l[1] - l[0]);
                if( val <= sq ) break;
                --l[0];
        }
        //cout << "l0: " << l[0] << "     highest level <= l1\n";

        l[2] = l[1] + 1;
        cout << "l[2]" << l[2] << '\n';
        while( l[2] < L.size() ){
                float val = L.at(l[2]) + 2*(l[2] - l[1] - 1);
                if( val <= snk ) break;
                ++l[2];
        }
        //cout << "l2: " << l[2] << "     lowest  level >= l1 + 1\n";

	return shrinktree(g_orig, g_copy, vis_data_orig, l); // step 6
}

// Step 4: l1_and_k
// Time:   O(n)
//
// Find the level l1 such that the total cost of levels 0 through l1 - 1 does not exceed 1/2,
// but the total cost of levels 0 through l1 does exceed 1/2.
// Let k be the number of vertices in levels 0 through l1
Partition l1_and_k(GraphCR g_orig, Graph& g_copy, vector<uint> const& L, BFSVisitorData const& vis_data_orig)
{
        //cout  << "---------------------------- 4 - l1 and k  ------------\n";
        uint k = L[0]; 
        uint l[3];
        uint n = num_vertices(g_copy);
        l[1] = 0;
        while( k <= n/2 ){
                uint indx = ++l[1];
                uint lsize = L.size();
                if( indx >= lsize ) break;
	       	k += L.at(indx);
	}

        /*cout << "k:  " << k    << "      # of verts in levels 0 thru l1\n";
        cout << "l1: " << l[1] << "      total cost of levels 0 thru l1 barely exceeds 1/2\n";*/
	assert(k <= n); 
	return find_more_levels(g_orig, g_copy, k, l, L, vis_data_orig); // step 5
}

// Step 3: bfs_and_levels
// Time:   O(n)
//
// Find a breadth-first spanning tree of the most costly component.
// Compute the level of each vertex and the number of vertices L(l) in each level l.
Partition bfs_and_levels(GraphCR g_orig, Graph& g_copy)
{
        cout << "---------------------------- 3 - BFS and Levels ------------\n";
        BFSVisitorData vis_data(&g_copy, *vertices(g_copy).first);
        breadth_first_search(g_copy, vis_data.root, boost::visitor(BFSVisitor(vis_data)));

        vector<uint> L(vis_data.num_levels + 1, 0);
	cout << "L levels: " << L.size() << '\n';
        for( auto& d : vis_data.verts ){
                cout << "level: " << d.second.level << '\n';
	       	++L[d.second.level];
	}

	VertIter vit, vjt;
        for( tie(vit, vjt) = vertices(g_copy); vit != vjt; ++vit ){
                if( vis_data.verts.contains(*vit) ) cout << "level/cost of vert " << *vit << ": " << vis_data.verts[*vit].level << '\n';
	}
        for( uint i = 0; i < L.size(); ++i ){
		cout << "L[" << i << "]: " << L[i] << '\n';
	}

	return l1_and_k(g_orig, g_copy, L, vis_data); // step 4
}

// Step 2: find_connected_components
// Time:   O(n)
//
// Find the connected components of G and determine the cost of each one.
// If none has cost exceeding 2/3, construct the partition as described in the proof of Theorem 4.
// If some component has cost exceeding 2/3, go to Step 3.
Partition find_connected_components(GraphCR g_orig, Graph& g_copy)
{
        //cout << "---------------------------- 2 - Find Connected Components --------\n";
        vertex_map idx; 
        associative_property_map<vertex_map> vertid_to_component(idx);
        VertIter vit, vjt;
        tie(vit, vjt) = vertices(g_copy);
        for( uint i = 0; vit != vjt; ++vit, ++i ){
		//cout << "checking vertex number: " << i << ' ' << *vit << '\n';
	       	put(vertid_to_component, *vit, i);
	}
        uint num_components = connected_components(g_copy, vertid_to_component);

        //cout << "# of connected components: " << num_components << '\n';
        vector<uint> num_verts_per_component(num_components, 0);
        for( tie(vit, vjt) = vertices(g_copy); vit != vjt; ++vit ){
	       	++num_verts_per_component[vertid_to_component[*vit]];
	}
        uint biggest_component_index = 0;
        uint biggest_size            = 0;
        bool bigger_than_two_thirds  = false;
        for( uint i = 0; i < num_components; ++i ){
                if( 3*num_verts_per_component[i] > 2*num_vertices(g_copy) ){
                        //cout << "component " << i << " is bigger than two thirds of the entire graph\n";
                        bigger_than_two_thirds = true;
                }
                if( num_verts_per_component[i] > biggest_size ){
                        biggest_size = num_verts_per_component[i];
                        biggest_component_index = i;
                }
        }

        if( !bigger_than_two_thirds ){
		//cout << "exiting early through theorem 4 - no component has cost exceeding two thirds\n";
                return theorem4(g_copy, vertid_to_component, num_verts_per_component);
        }
        //cout << "index of biggest component: " << biggest_component_index << '\n';

	return bfs_and_levels(g_orig, g_copy); // step 3
}

// Step 1: check_planarity
// Time:   big-theta(n)
//
// Find a planar embedding of G and construct a representation for it of the kind described above.
Partition lipton_tarjan(GraphCR g_orig)
{
	Graph g_copy(g_orig);
	copy_graph(g_orig, g_copy);
	g_copy = g_orig;

	//cout << "@#$original g:\n";
	//print_graph(g_orig);
	//cout << "@#$g_copy:\n";
        //print_graph_special(g_copy, vmap_copy);
	//print_graph2(g_copy);

        auto prop_map = get(vertex_index, g_copy);
        VertIter vi, vend;
        for( tie(vi, vend) = vertices(g_copy); vi != vend; ++vi ){
                cout << "vert#: " << prop_map[*vi] << '\n';
        }

	/*cout << "---------------------------- 0 - Printing Edges -------------------\n";
	cout << "edges of g:\n";*/
	//print_edges(g_orig, vmap);
	//cout << "edges of g_copy:\n" << std::endl;
	//print_edges(g_copy, vmap_copy);

        //cout << "---------------------------- 1 - Check Planarity  ------------\n";
        EmbedStruct em(&g_copy);
        if( !em.test_planar() ) throw NotPlanarException();

	return find_connected_components(g_orig, g_copy);
}
