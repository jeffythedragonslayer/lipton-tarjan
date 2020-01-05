#include "Partition.h"
#include "typedefs.h"
#include <boost/graph/properties.hpp>
#include <iostream>
#include <algorithm>
using namespace std;

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