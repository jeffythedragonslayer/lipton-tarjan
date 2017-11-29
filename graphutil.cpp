#include "graphutil.h"
#include "BFSVisitorData.h"
#include "strutil.h"
#include <iostream>
#include <boost/graph/make_biconnected_planar.hpp>
#include <boost/graph/make_maximal_planar.hpp>
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

VertDesc common_ancestor(vector<VertDesc> const& ancestors_v, vector<VertDesc> const& ancestors_w)
{
        for( uint i = 0; i < ancestors_v.size(); ++i ){
		for( uint j = 0; j < ancestors_w.size(); ++j ){
			if( ancestors_v[i] == ancestors_w[j] ) return ancestors_v[i];
		}
	}
	assert(0);
        return VertDesc();
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
        auto parents_v = ancestors(v, vis_data);
        auto parents_w = ancestors(w, vis_data); 
        auto ancestor  = common_ancestor(parents_v, parents_w);
        cout << "common ancestor: " << ancestor << '\n'; 
        return get_cycle(v, w, ancestor, vis_data);
}

// return set of vertices neighboring v in graph g
set<VertDesc> get_neighbors(VertDesc v, Graph const& g)
{ 
        set<VertDesc> neighbors;
        OutEdgeIter e_cur, e_end;
        for( tie(e_cur, e_end) = out_edges(v, g); e_cur != e_end; ++e_cur ){
		auto n = target(*e_cur, g);
		neighbors.insert(n);
		cout << "      vertex " << v << " has neighbor " << n << '\n';
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

CycleCost compute_cycle_cost(vector<VertDesc> const& cycle, Graph const& g, BFSVisitorData const& vis_data, EmbedStruct const& em)
{
        CycleCost cc;
        for( auto& v : cycle ){
                //cout << "   scanning cycle vert " << v << '\n';
                for( auto e = out_edges(v, g); e.first != e.second; ++e.first ) if( vis_data.is_tree_edge(*e.first) && !on_cycle(*e.first, cycle, g) ){
                        uint cost = vis_data.edge_cost(*e.first, cycle, g);
                        //cout << "      scanning incident tree edge " << to_string(*e.first, g) << "   cost: " << cost << '\n';
                        auto insideout = edge_inside_cycle(*e.first, v, cycle, g, em.em);
                        assert(insideout != ON);
                        bool is_inside = (insideout == INSIDE);
                        (is_inside ? cc.inside : cc.outside) += cost;
                }
        }
        return cc;
}

void make_max_planar(Graph& g)
{ 
        auto index = reset_edge_index(g);
        EmbedStruct em(&g);
        em.test_planar();
        make_biconnected_planar(g, em.em, index);

        reset_edge_index(g);
        em.test_planar();

        make_maximal_planar(g, em.em);

        reset_edge_index(g);
        assert(em.test_planar());
} 

void reset_vertex_indices(Graph& g)
{
        VertIter vi, vend;
        uint i = 0;
        for( tie(vi, vend) = vertices(g); vi != vend; ++vi, ++i ){
		put(vertex_index, g, *vi, i); 
	}
}

EdgeIndex reset_edge_index(Graph const& g)
{
        EdgeIndex edgedesc_to_uint; 
        EdgesSizeType num_edges = 0;
        EdgeIter ei, ei_end;
        for( tie(ei, ei_end) = edges(g); ei != ei_end; ++ei ){
	       	edgedesc_to_uint[*ei] = num_edges++;
	}
        return edgedesc_to_uint;
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
