//=======================================================================
// Copyright 2015 - 2020 Jeff Linahan
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include "graphutil.h"
#include "BFSVisitorData.h"
#include "strutil.h"
#include <boost/graph/make_biconnected_planar.hpp>
#include <boost/graph/make_maximal_planar.hpp>
#include <boost/algorithm/string.hpp> 
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <fstream>
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

void init_vert_propmap(Graph& g)
{
	auto prop_map = get(vertex_index, g); // writing to this property map has side effects in the graph
	VertIter vi, vend;
	uint i = 0;
	for( tie(vi, vend) = vertices(g); vi != vend; ++vi ){
#ifdef GRAPH_TYPE_VEC
                abort();
#else
		prop_map[*vi] = i;
#endif
		cout << "prop map " << *vi << " == " << i << '\n';
		++i;
	} 
}

vertex_t get_common_ancestor(vector<vertex_t> const& ancestors_v, vector<vertex_t> const& ancestors_w)
{
        for( uint i = 0; i < ancestors_v.size(); ++i ){
		for( uint j = 0; j < ancestors_w.size(); ++j ){
			if( ancestors_v[i] == ancestors_w[j] ) return ancestors_v[i];
		}
	}
        return Graph::null_vertex();
}

// return set of vertices neighboring v in graph g
set<vertex_t> get_neighbors(vertex_t v, Graph const& g)
{ 
        set<vertex_t> neighbors;
        OutEdgeIter e_cur, e_end;
        for( tie(e_cur, e_end) = out_edges(v, g); e_cur != e_end; ++e_cur ){
		auto n = target(*e_cur, g);
		neighbors.insert(n);
		//cout << "      vertex " << vmap.vert2uint[v] << " has neighbor " << vmap.vert2uint[n] << '\n';
	}
        return neighbors;
}

// get set intersection of a and b
set<vertex_t> get_intersection(set<vertex_t> const& a, set<vertex_t> const& b)
{
        set<vertex_t> c;
        set_intersection(STLALL(a), STLALL(b), inserter(c, c.begin())); 
        //for( auto& i : c ) cout << "      set intersection: " << vmap.vert2uint[i] << '\n'; 
        return c;
} 

/* Given an edge e and a cycle of vertices, determine whether e is in inside, outside, or on the cycle.
An embedding is needed to establish what is inside and outside.  
e may be anywhere on the graph.
common_vert_on_cycle should be a tree vertex that both of e's incident vertices share as an ancestor */
InsideOutOn is_edge_inside_outside_or_on_cycle(edge_t e, vertex_t common_vert_on_cycle, vector<vertex_t> const& cycle, Graph const& g, Embedding const& em)
{
        cout << "------------------ is edge inside outside or on cycle -----------------\n";
        cout << "is edge inside outside or on cycle? " << to_string(e, g) << '\n';
	//cout << "edge: " << vmap.vert2uint[source(e, g)] << ' ' << vmap.vert2uint[target(e, g)] << '\n';
        //cout << "cycle: ";
        //for( uint i = 0; i < cycle.size(); ++i ) cout << vmap.vert2uint[cycle[i]] << ' ';
        //cout << '\n';

        auto src = source(e, g);
        auto tar = target(e, g);
        if( on_cycle(e, cycle, g) ){
		cout << "edge is on cycle\n";
		return ON;
	}
        //cout << "      testing if edge " << vmap.vert2uint[src] << ", " << vmap.vert2uint[tar] << " is inside or outside the cycle\n";
        //cout << "      common_vert_on_cycle:    " << vmap.vert2uint[common_vert_on_cycle]   << '\n';
        auto it = find(STLALL(cycle), common_vert_on_cycle);
        if( it == cycle.end() ){ cout << "      common_vert_on_cycle needs to appear in cycle\n"; BOOST_ASSERT(0); }
        BOOST_ASSERT(*it == common_vert_on_cycle);
        auto before_common = it   == cycle.begin() ?  cycle.end  ()-1   : it-1;
        auto after_common        = it+1 == cycle.end  () ?  cycle.begin()     : it+1; 
        auto other_end_of_common = source(e, g) == common_vert_on_cycle ? target(e, g) : source(e, g); 
        
        /*cout << '\n';
        cout << "      it:     " << vmap.vert2uint[*it]                  << '\n';
        cout << "      v:      " << vmap.vert2uint[common_vert_on_cycle] << '\n';
        cout << "      before: " << vmap.vert2uint[*before_common]       << '\n';
        cout << "      after:  " << vmap.vert2uint[*after_common]        << '\n';
        cout << "      other:  " << vmap.vert2uint[other_end_of_common]  << '\n';*/

        vector<uint> permu;
        set<vertex_t> seenbefore;
        for( auto& edge : em[*it] ){ // why does this contain duplicates?
                auto src = source(edge, g);
                auto tar = target(edge, g);
                if( src != common_vert_on_cycle ) swap(src, tar);
                BOOST_ASSERT(src == common_vert_on_cycle);

                //cout << "    tar: " << vmap.vert2uint[tar] << '\n';

                if( seenbefore.find(tar) != seenbefore.end() ) continue;
                seenbefore.insert(tar);

                // this can't be an if-else ladder because sometimes other, before, and after equal each other and we wouldn't have 3 permutations
                if( tar == other_end_of_common ) permu.push_back(1);
                if( tar == *before_common      ) permu.push_back(2);
                if( tar == *after_common       ) permu.push_back(3);
        } 
        BOOST_ASSERT(permu.size() == 3);

	return levi_civita(permu[0], permu[1], permu[2]) == 1 ?
	       INSIDE					      :
	       OUTSIDE;
}


// scan edges around all vertices of cycle and add up edge costs
CycleCost compute_cycle_cost(vector<vertex_t> const& cycle, Graph const& g, BFSVisitorData const& vis_data, EmbedStruct const& em)
{
        CycleCost cc;
        for( auto& v : cycle ){
                cout << "   scanning cycle vert " << v << '\n';
                for( auto e = out_edges(v, g); e.first != e.second; ++e.first ){
                        if( vis_data.is_tree_edge(*e.first) ){
                                if( !on_cycle(*e.first, cycle, g) ){
                                        uint cost = vis_data.edge_cost(*e.first, cycle, g);
                                        //uint vert_id = vmap.vert2uint[v];
                                        //cout << "      scanning incident tree edge " << vert_id << "   cost: " << cost << '\n';
                                        auto insideout = is_edge_inside_outside_or_on_cycle(*e.first, v, cycle, g, em.em);
                                        BOOST_ASSERT(insideout != ON);
                                        bool is_inside = (insideout == INSIDE);
                                        (is_inside ? cc.inside : cc.outside) += cost;
                                }
                        }
                }
        }
        return cc;
}

// Make all faces of graph G into triangles by adding scanning the boundary and adding (nontree) edges as necessary
EmbedStruct make_max_planar(Graph& g)
{ 
        EdgeIndex i = reset_edge_index(g);
        EmbedStruct em(&g);
        BOOST_ASSERT(em.test_planar());
        make_biconnected_planar(g, em.em, i);

        reset_edge_index(g);
        BOOST_ASSERT(em.test_planar());

        make_maximal_planar(g, em.em);

        reset_edge_index(g);
        BOOST_ASSERT(em.test_planar());
        return em;
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
        EdgesSizeType edge_num = 0;
        EdgeIter ei, ei_end;
        for( tie(ei, ei_end) = edges(g); ei != ei_end; ++ei ){
	       	edgedesc_to_uint[*ei] = edge_num++;
	}
        return edgedesc_to_uint;
} 

struct CantContractVertexWithSelf
{ 
};

// replace a and b with a single vertex that has edges to all of their neighbors
vertex_t contract_vertices(vertex_t a, vertex_t b, Graph& g)
{
        if( a == b ) throw CantContractVertexWithSelf();
        auto be = adjacent_vertices(b, g);
        for( auto beit = be.first; beit != be.second; ++beit ) if( *beit != a ) add_edge(a, *beit, g);
        remove_vertex(b, g);
        return a;
}

void kill_vertex(vertex_t v, Graph& g)
{
        //cout << "killing vertex " << i << '\n';
        clear_vertex(v, g);
        remove_vertex(v, g);
}

struct FileNotFound {};

Graph load_graph(string const& fname)
{
        ifstream in(fname);
        if( !in ){
                cerr << "file \"in\" not found!\n";
		throw FileNotFound();
        }

        string str;
        vector<pair<uint, uint>> file_edges;
        int max_v = -1;
        while( getline(in, str) ){
                uint   colon = str.find(","); 
                string stra  = str.substr(0, colon); trim(stra);
                string strb  = str.substr(colon+1 ); trim(strb); 
                int   a     = lexical_cast<int>(stra);
                int   b     = lexical_cast<int>(strb);
                max_v = max(max(max_v, a), b);
                file_edges.push_back({a, b});
        } 
        Graph g(max_v+1);

	map<uint, vertex_t> uint_to_vert;

	VertIter vi, vi_end;
	uint i = 0;
	for( tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi ){
		//put(vertex_index, g, *vi, i); 
		uint_to_vert[i] = *vi;
		++i;
	}

        for( pair<uint, uint>& e : file_edges ){
                auto src = uint_to_vert[e.first];
                auto tar = uint_to_vert[e.second];
                add_edge(src, tar, g);
        }

        return g;
}

bool vertex_exists(vertex_t x, Graph const& g)
{ 
        VertIter vi, vi_end;
        for( tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
                if( *vi == x ) return true;
        }
        return false; 
}

bool edge_exists(edge_t e, Graph const& g)
{
        EdgeIter ei, ei_end;
        for( tie(ei, ei_end) = edges(g); ei != ei_end; ++ei ){
                if( *ei == e ) return true; 
        }
        return false; 
}

bool assert_verts(GraphCR g, BFSVisitorData const& vis_data)
{
        VertIter vei, vend;
        for( tie(vei, vend) = vertices(g); vei != vend; ++vei ){ 
                vertex_t v = *vei;
                if( !vis_data.verts.contains(v) ){
                        cout << "graphutils.cpp: ignoring bad vertex : " << v << '\n';
                        return false;
                }
        } 
        return true;
}
