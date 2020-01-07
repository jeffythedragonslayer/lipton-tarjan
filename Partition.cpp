#include "Partition.h"
#include "EmbedStruct.h"
#include "typedefs.h"
#include "BFSVisitorData.h"
#include "BFSVisitor.h"
#include <boost/graph/properties.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using namespace boost;

struct VertBuffer : boost::queue<vertex_t>
{

};

Partition::Partition(vector<vertex_t> const& cycle, Graph& g, EmbedStruct const& em)
{
		vector<int> colormap(num_vertices(g), boost::white_color);
		// mark all verts on the cycle as VISITED (white)
		// mark all other verts as NOT VISITED
		c = set<vertex_t>(STLALL(cycle));

		auto[vit, vjt] = vertices(g);
		auto l = [&](vertex_t x){return find(STLALL(cycle), x) != cycle.end();};

		auto noncycle = std::find_if(vit, vjt, l);
		BFSVisitorData visdata(&g, *noncycle);
		BFSVisitor vis(visdata);

		VertBuffer q;
		//auto visitor = boost::visitor(BFSVisitor(visdata));
		//put(vertex_color, g, *vit, boost::white_color);
		auto colormap2 = get(vertex_index, g); // replace this with vertex_color
		breadth_first_visit(g, visdata.root, q, visdata, colormap2);
		// collect all visited verts into A partition

		// pick an arbitrary noncycle vert that is still unvisited and do a BFS, 
		breadth_first_visit(g, visdata.root, q, visdata, colormap2);
		// collect all visited verts in B partition
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