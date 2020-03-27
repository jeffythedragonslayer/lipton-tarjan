#include "Partition.h"
#include "EmbedStruct.h"
#include "typedefs.h"
#include "strutil.h"
#include "BFSVisitorData.h"
#include "BFSVisitor.h"
#include <boost/graph/properties.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/config.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/connected_components.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using namespace boost;

struct VertBuffer : boost::queue<vertex_t>
{

};

set<vertex_t> g_collected;

struct CollectBFS : public BFSVisitorData
{
		CollectBFS(Graph const* g, vertex_t root) : BFSVisitorData(g, root) {cout << "ctor";};
		CollectBFS(CollectBFS const& other) : CollectBFS(other.g, other.root) {};
		virtual ~CollectBFS() {};

		//set<vertex_t> collected;
		virtual void initialize_vertex(vertex_t v, GraphCR g) {cout << "init vertex" << v << '\n';};
		virtual void examine_vertex(vertex_t v, GraphCR g) {cout << "examine vertex" << v << '\n';};

		virtual void discover_vertex(vertex_t v, GraphCR g)
		{
				cout << "discovering vertex: " << v << '\n';
				g_collected.insert(v);
				cout << "collector size: " << g_collected.size() << '\n';
		};

		virtual void finish_vertex(vertex_t v, GraphCR g) {cout << "finish vertex: " << v << '\n';};
		virtual void black_target(edge_t, GraphCR g) {cout << "BLACK TARGET\n";};
		virtual void gray_target(edge_t, GraphCR g) {cout << "GRAY TARGET\n";};
		virtual void tree_target(edge_t, GraphCR g) {};
		virtual void tree_edge(edge_t, GraphCR g) {}; 
		virtual void non_tree_edge(edge_t, GraphCR g) {};
};

// A = verts inside cycle
// B = verts outside cycle
// C = verts on cycle
Partition::Partition(vector<vertex_t> const& cycle, Graph& g, EmbedStruct const& em)
{ 
		c = set<vertex_t>(STLALL(cycle));

		for( auto v : c ) clear_vertex(v, g);

		uint n = num_vertices(g);

        vertex_map idx; 
        associative_property_map<vertex_map> vertid_to_component(idx);
        auto[vit, vjt] = vertices(g);
        for( uint i = 0; vit != vjt; ++vit, ++i ){
				cout << "checking vertex number: " << i << ' ' << *vit << '\n';
				put(vertid_to_component, *vit, i);
		}
        uint num_components = connected_components(g, vertid_to_component);
		cout << "num_components: " << num_components << '\n';

		assert(2 == num_components-cycle.size()); // each vertex on the cycle is now its own component, leaving only A and B left

        //cout << "# of connected components: " << num_components << '\n';
        for( tie(vit, vjt) = vertices(g); vit != vjt; ++vit ){
				if( find(STLALL(cycle), *vit) != cycle.end() ){
					cout << "vert: " << *vit << " is in C\n";
					continue; // *vit is part of C
				}

				int comp = vertid_to_component[*vit];
				cout << "comp#: " << comp << '\n';
				if( 1 == comp ){
						b.insert(*vit);
				} else {
						a.insert(*vit);
				}
		} 

}

void Partition::get_most_costly_part(set<vertex_t> const** most_costly,
									set<vertex_t> const** other1,
									set<vertex_t> const** other2) const
{
	if( a.size() >= b.size() && a.size() >= c.size() ){
		*most_costly = &a;
		*other1      = &b;
		*other2      = &c;
		return;
	}
	if( b.size() >= a.size() && b.size() >= c.size() ){
		*most_costly = &b;
		*other1      = &a;
		*other2      = &c;
		return;
	}
	if( c.size() >= a.size() && c.size() >= b.size() ){
		*most_costly = &c;
		*other1      = &a;
		*other2      = &b;
		return;
	}
	assert(0);
}

bool Partition::verify_sizes_lemma3(vector<uint> const& L, uint l1, uint l2) const
{
		// verify that neither a nor b is bigger than two thirds of the total and c is no bigger than L(l1) + L(l2) + max{0, 2(l2-l1-1)}
		cout << "verifying partition sizes\n";
		uint a_verts = a.size();
		uint b_verts = b.size();
		uint c_verts = c.size();
		uint n  = a_verts + b_verts + c_verts;

		uint maxc = L[l1] + L[l2] + std::max<int>(0, 2*((int)l2-(int)l1-1));

		return a_verts <= 2*n/3 && 
			b_verts <= 2*n/3 && 
			c_verts <= maxc;
}

bool Partition::verify_sizes_lemma2(uint r, vertex_t root) const
{
		uint a_verts = a.size();
		uint b_verts = b.size();
		uint c_verts = c.size();
		uint n = a_verts + b_verts + c_verts;

		return a_verts <= 2*n/3 &&
			b_verts <= 2*n/3 &&
			c_verts <= 2*r+1 &&
			c.contains(root); 
}

bool Partition::verify_edges(GraphCR g) const
{
	// verify that no edge joins a vertex in partition A with a vertex in partition B
	EdgeIter ei, ei_end;
	for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei){
		auto v1 = source(*ei, g);
		auto v2 = target(*ei, g);

		if( find(STLALL(a), v1) != a.end()){ // if v1 is in a
			if( find(STLALL(b), v2) != b.end() ) return false;// v2 should not be in b
		} else if( find(STLALL(b), v1) != b.end() ){ // if v1 is in b
			if( find(STLALL(a), v2) != a.end() ) return false; // v2 should not be in a
		}
	}

	return true;
}

bool Partition::verify_sizes(GraphCR g) const
{
	// verify that neither a nor b is bigger than two thirds of the total and c is no bigger than 2*sqrt(2)*sqrt(n)
	// also verify that partitions have the same total number of verts as the graph
	cout << "verifying partition sizes\n";
	uint a_verts = a.size();
	uint b_verts = b.size();
	uint c_verts = c.size();
	uint p_total  = a_verts + b_verts + c_verts;
	uint n = num_vertices(g);
	uint abbound = (2*n) / 3;
	uint cbound = 2*sqrt(2)*sqrt(n);
	if( n != p_total ) return false;

	if( a_verts > abbound ) return false;
	if( b_verts > abbound ) return false; 
	if( c_verts > cbound ) return false;
	return true;
} 

void Partition::print(Graph const* g) const
{
		cout << "Partition\n"; 
		cout << "  size of A: " << a.size() << '\n';
		cout << "  size of B: " << b.size() << '\n';
		cout << "  size of C: " << c.size() << '\n'; 
		if( g ){
				auto prop_map = get(boost::vertex_index, *g); // writing to this property map has side effects in the graph
				cout << "  A = "; for( auto& v : a ) cout << prop_map[v] << ' '; cout << '\n';
				cout << "  B = "; for( auto& v : b ) cout << prop_map[v] << ' '; cout << '\n'; 
				cout << "  C = "; for( auto& v : c ) cout << prop_map[v] << ' '; cout << '\n';
		} else {
				cout << "  A = "; for( auto& v : a ) cout << v << ' '; cout << '\n';
				cout << "  B = "; for( auto& v : b ) cout << v << ' '; cout << '\n'; 
				cout << "  C = "; for( auto& v : c ) cout << v << ' '; cout << '\n';
		}
}

uint Partition::total_num_verts() const
{
		return a.size() + b.size() + c.size();
}