#include "BFSVisitorData.h"
#include "lipton-tarjan.h"
#include "strutil.h"
#include "graphutil.h"
#include <iostream>
using namespace std;

bool on_cycle(vertex_t v, vector<vertex_t> const& cycle, Graph const& g)
{
        return find(STLALL(cycle), v) != cycle.end();
}

bool on_cycle(edge_t e, vector<vertex_t> const& cycle, Graph const& g)
{
        auto src = source(e, g);
        auto tar = target(e, g);
        return on_cycle(src, cycle, g) &&
	           on_cycle(tar, cycle, g);
}

BFSVisitorData::BFSVisitorData(Graph const* g, vertex_t root) : g(g), num_levels(0), root(root)
{
        assert(vertex_exists(root, *g));
	verts[root] = BFSVert();
	children[root] = set<vertex_t>(); 
}

void BFSVisitorData::reset(Graph* g)
{
        children.clear();
        verts   .clear();
        num_levels = 0;
        this->g = g;
        root = Graph::null_vertex();
}

struct EdgeOops
{
};

void BFSVisitorData::examine_edge(edge_t e)
{
	tree_edges.push_back(e);
	throw EdgeOops();
}

bool BFSVisitorData::is_tree_edge(edge_t e) const
{
	return find(STLALL(tree_edges), e) != tree_edges.end();
}

bool BFSVisitorData::in_cc(edge_t e) const
{ 
		vertex_t src = source(e, *g);
		vertex_t tar = target(e, *g); 
		auto src_it = this->verts.find(src);
		auto tar_it = this->verts.find(tar);
		//if (vmap) cout << "testing is edge in cc (" << vmap->vert2uint[src] << ", " << vmap->vert2uint[tar] << ")\n";
		if( src_it == verts.end() || tar_it == verts.end() ){
				//cout << "edge has src or tar not appearing in BFSVisitorData verts: (" << vmap->vert2uint[src] << ", " << vmap->vert2uint[tar] << ")\n";
				return false;
		}
		return src_it->second.parent == tar || tar_it->second.parent == src; // tree edges will have a parent-child relationship among their source and target but we're not sure which is which
}

uint BFSVisitorData::edge_cost(edge_t e, vector<vertex_t> const& cycle, Graph const& g) const
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

void BFSVisitorData::print_costs  () const {for( auto& v : verts ) cout << "descendant cost of vertex " << v.first << " is " << v.second.descendant_cost << '\n';}
void BFSVisitorData::print_parents() const {for( auto& v : verts ) cout << "parent of " << v.first << " is " << v.second.parent << '\n';}


// make sure all verts appear in the graph
bool BFSVisitorData::assert_data() const
{
        auto prop_map = get(boost::vertex_index, *g); // writing to this property map has side effects in the graph

        VertIter vei, vend;
        for( tie(vei, vend) = vertices(*g); vei != vend; ++vei ){ 
                vertex_t v = *vei;
                if( !this->verts.contains(v) ){
                        cout << "found bad vertex : " << v << " prop_map: " << prop_map[v] << '\n';
			return false;
                }
        }
		return true;
}

struct NoCommonAncestor {};

vector<vertex_t> BFSVisitorData::get_cycle(vertex_t v, vertex_t w, vertex_t ancestor) const
{
        if( !ancestor ) throw NoCommonAncestor();
        vector<vertex_t> cycle, tmp;
        vertex_t cur;
        cur = v;
        while( cur != ancestor ){
                cycle.push_back(cur);
                auto cur_it = verts.find(cur);
                cur = cur_it->second.parent;
        }
        cycle.push_back(ancestor); 

        cur = w;
        while( cur != ancestor ){
                tmp.push_back(cur);
                auto cur_it = verts.find(cur);
                cur = cur_it->second.parent;
        }

        reverse(STLALL(tmp));
        cycle.insert(cycle.end(), STLALL(tmp));
        return cycle;
}

// returns the first nontree edge we fin
edge_t BFSVisitorData::arbitrary_nontree_edge(Graph const& g) const
{
	cout << "starting arbitrary_nontree_edge function\n";
        EdgeIter ei, ei_end;
	uint num_edges = 0;
        for( tie(ei, ei_end) = edges(g); ei != ei_end; ++ei, ++num_edges ){
                auto src = source(*ei, g);
                auto tar = target(*ei, g);
                assert(edge(src, tar, g).second); // edge exists
				//cout << "candidate edge: " << vmap.vert2uint[src] << ' ' << vmap.vert2uint[tar] << '\n';
                if( src == tar ){
                        cout << "ignoring circular edge\n";
                        continue;
                        //kthrow FoundCircularNode(src);
                }

                if( !in_cc(*ei) ) continue;
                try {

                        if( !is_tree_edge(*ei) ){
                                cout << "found nontree edge\n";
                                cout << "total edges looked at: " << (1+num_edges) << '\n';
                                cout << "arbitrarily choosing nontree edge: " << to_string(*ei, g) << '\n';
                                return *ei; 
                        } else cout << "is a tree edge\n";

                } catch (EdgeNotInVisitorData& e){
                        cout << "edge not in visitor data\n";
                }
		++num_edges;
        }
        throw NoNontreeEdgeException(num_edges); 
}

bool BFSVisitorData::assert_verts(GraphCR g) const
{
        VertIter vei, vend;
        for( tie(vei, vend) = vertices(g); vei != vend; ++vei ){ 
                vertex_t v = *vei;
                if( !verts.contains(v) ){
                        cout << "graphutils.cpp: ignoring bad vertex : " << v << '\n';
                        return false;
                }
        } 
        return true;
}

vector<vertex_t> BFSVisitorData::ancestors(vertex_t v) const
{
        vector<vertex_t> ans;
        while( v ){
                ans.push_back(v);
                v = verts.find(v)->second.parent;
        }
        return ans;
}

vector<vertex_t> BFSVisitorData::get_cycle(vertex_t v, vertex_t w) const
{ 
        vector<vertex_t> parents_v = ancestors(v);
        vector<vertex_t> parents_w = ancestors(w); 
        vertex_t ancestor  = get_common_ancestor(parents_v, parents_w);
        //cout << "common ancestor: " << ancestor << '\n'; 
        return get_cycle(v, w, ancestor);
}
vector<vertex_t> BFSVisitorData::get_cycle(edge_t e) const
{ 
        vertex_t v = source(e, *g);
        vertex_t w = target(e, *g);
        vector<vertex_t> parents_v = ancestors(v);
        vector<vertex_t> parents_w = ancestors(w); 
        return get_cycle(v, w, get_common_ancestor(parents_v, parents_w));
}