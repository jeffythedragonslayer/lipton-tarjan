#include "graphutil.h"
#include "BFSVisitorData.h"
#include "strutil.h"
#include <iostream>
#include <fstream>
#include <boost/graph/make_biconnected_planar.hpp>
#include <boost/graph/make_maximal_planar.hpp>
#include <boost/algorithm/string.hpp> 
#include <boost/lexical_cast.hpp>
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

vertex_t common_ancestor(vector<vertex_t> const& ancestors_v, vector<vertex_t> const& ancestors_w)
{
        for( uint i = 0; i < ancestors_v.size(); ++i ){
		for( uint j = 0; j < ancestors_w.size(); ++j ){
			if( ancestors_v[i] == ancestors_w[j] ) return ancestors_v[i];
		}
	}
	assert(0);
        return vertex_t();
}

vector<vertex_t> ancestors(vertex_t v, BFSVisitorData const& vis)
{
        //cout << "first v: " << v << '\n';
        //cout << "root: " << vis.root << '\n';
        vector<vertex_t> ans = {v};
        while( v != vis.root ){
                auto v_it = vis.verts.find(v);
                assert(v_it != vis.verts.end());
                v = v_it->second.parent;
                ans.push_back(v);
                //cout << "pushing back v: " << v << '\n';
        }
        return ans;
}

vector<vertex_t> get_cycle(vertex_t v, vertex_t w, vertex_t ancestor, BFSVisitorData const& vis_data)
{
        vector<vertex_t> cycle, tmp;
        vertex_t cur;
        cur = v; while( cur != ancestor ){ cycle.push_back(cur); auto cur_it = vis_data.verts.find(cur); cur = cur_it->second.parent; } cycle.push_back(ancestor); 
        cur = w; while( cur != ancestor ){ tmp  .push_back(cur); auto cur_it = vis_data.verts.find(cur); cur = cur_it->second.parent; }
        reverse(STLALL(tmp));
        cycle.insert(cycle.end(), STLALL(tmp));
        return cycle;
}

vector<vertex_t> get_cycle(vertex_t v, vertex_t w, BFSVisitorData const& vis_data)
{ 
        auto parents_v = ancestors(v, vis_data);
        auto parents_w = ancestors(w, vis_data); 
        auto ancestor  = common_ancestor(parents_v, parents_w);
        cout << "common ancestor: " << ancestor << '\n'; 
        return get_cycle(v, w, ancestor, vis_data);
}

// return set of vertices neighboring v in graph g
set<vertex_t> get_neighbors(vertex_t v, Graph const& g)
{ 
        set<vertex_t> neighbors;
        OutEdgeIter e_cur, e_end;
        for( tie(e_cur, e_end) = out_edges(v, g); e_cur != e_end; ++e_cur ){
		auto n = target(*e_cur, g);
		neighbors.insert(n);
		cout << "      vertex " << v << " has neighbor " << n << '\n';
	}
        return neighbors;
}

set<vertex_t> get_intersection(set<vertex_t> const& a, set<vertex_t> const& b)
{
        set<vertex_t> c;
        set_intersection(STLALL(a), STLALL(b), inserter(c, c.begin())); 
        for( auto& i : c ) cout << "      set intersection: " << i << '\n'; 
        assert(c.size() == 2);
        return c;
} 

InsideOutOn edge_inside_outside_cycle(edge_t e, vertex_t common_vert, vector<vertex_t> const& cycle, Graph const& g, Embedding const& em)
{
        cout << "cycle: ";
        for( uint i = 0; i < cycle.size(); ++i ) cout << cycle[i] << ' ';
        cout << '\n';
        auto src = source(e, g);
        auto tar = target(e, g);
        if( on_cycle(e, cycle, g) ) return ON;
        cout << "      testing if edge " << src << ", " << tar << " is inside the cycle: ";
        cout << "      common_vert:    " << common_vert   << '\n';
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
        set<vertex_t> seenbefore;
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

edge_t arbitrary_nontree_edge(Graph const& g, Vert2UintMap& vmap, BFSVisitorData const& vis_data)
{ 
	cout << "starting arbitrary_nontree_edge function\n";
        EdgeIter ei, ei_end;
	uint i = 0;
        for( tie(ei, ei_end) = edges(g); ei != ei_end; ++ei ){
                auto src = source(*ei, g);
                auto tar = target(*ei, g);
		cout << "candidate edge: " << src << ' ' << tar << '\n';
                assert(edge(src, tar, g).second); // exists
                assert(src != tar); 
                if( !vis_data.is_tree_edge(*ei) ) break;
		++i;
        }
	cout << "total edges: " << i << '\n';
        if( ei == ei_end ) throw NoNontreeEdgeException(i);

        assert(!vis_data.is_tree_edge(*ei));
        edge_t chosen_edge = *ei;
        cout << "arbitrarily choosing nontree edge: " << to_string(chosen_edge, vmap, g) << '\n';
        return chosen_edge;
}

CycleCost compute_cycle_cost(vector<vertex_t> const& cycle, Graph const& g, BFSVisitorData const& vis_data, EmbedStruct const& em)
{
        CycleCost cc;
        for( auto& v : cycle ){
                //cout << "   scanning cycle vert " << v << '\n';
                for( auto e = out_edges(v, g); e.first != e.second; ++e.first ) if( vis_data.is_tree_edge(*e.first) && !on_cycle(*e.first, cycle, g) ){
                        uint cost = vis_data.edge_cost(*e.first, cycle, g);
                        //cout << "      scanning incident tree edge " << to_string(*e.first, g) << "   cost: " << cost << '\n';
                        auto insideout = edge_inside_outside_cycle(*e.first, v, cycle, g, em.em);
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

void kill_vertex(vertex_t v, Graph& g, Vert2UintMap& vmap)
{
        cout << "killing vertex " << v << '\n';
        auto i = vmap.vert2uint[v];
        vmap.uint2vert.erase(i);
        vmap.vert2uint.erase(v);
        clear_vertex(v, g);
        remove_vertex(v, g);
}

void create_vmap_from_graph(Graph const& g, Vert2UintMap& vmap)
{
        VertIter vi, vi_end;
        uint i = 0;
        for( tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi ){
                vmap.vert2uint[*vi] = i;
                vmap.uint2vert[i] = *vi;
		vmap.vu_bimap.insert({*vi, i});
                ++i;
        }
}

struct FileNotFound {};

Graph load_graph(string const& fname, Vert2UintMap& vmap)
{
        ifstream in(fname);
        if( !in ){
                cerr << "file \"in\" not found!\n";
		throw FileNotFound();
        }

        string str;
        vector<pair<uint, uint>> edges;
        int max_v = -1;
        while( getline(in, str) ){
                uint   colon = str.find(","); 
                string stra  = str.substr(0, colon); trim(stra);
                string strb  = str.substr(colon+1 ); trim(strb); 
                int   a     = lexical_cast<int>(stra);
                int   b     = lexical_cast<int>(strb);
                max_v = max(max(max_v, a), b);
                edges.push_back(make_pair(a, b));
        } 
        Graph g(max_v+1);
        VertIter vi, vi_end;
        uint i = 0;
        for( tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi ){
                vmap.vert2uint[*vi] = i;
                vmap.uint2vert[i] = *vi;
		vmap.vu_bimap.insert({*vi, i});
                ++i;
        }
        for( auto& e : edges ){
                auto src = vmap.uint2vert[e.first];
                auto tar = vmap.uint2vert[e.second];
		//map<uint, vertex_t> m = vu_bimap.left;
                add_edge(src, tar, g);
        }

        vmap.vert2uint[Graph::null_vertex()] = -1;
	vmap.vu_bimap.insert({Graph::null_vertex(), static_cast<uint>(-1)});
        return g;
}
