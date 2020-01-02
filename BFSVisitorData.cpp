#include "BFSVisitorData.h"
#include "lipton-tarjan.h"
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

bool BFSVisitorData::is_tree_edge(edge_t e) const
{ 
		vertex_t src = source(e, *g);
		vertex_t tar = target(e, *g); 
		auto src_it = this->verts.find(src);
		auto tar_it = this->verts.find(tar);
		//if (vmap) cout << "testing is tree edge: (" << vmap->vert2uint[src] << ", " << vmap->vert2uint[tar] << ")\n";
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
        VertIter vei, vend;
        for( tie(vei, vend) = vertices(*g); vei != vend; ++vei ){ 
                vertex_t v = *vei;
                if( !this->verts.contains(v) ){
                        cout << "found bad vertex : " << v << '\n';
						return false;
                }
        }
		return true;
}