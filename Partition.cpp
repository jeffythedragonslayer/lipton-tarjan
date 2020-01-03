#include "Partition.h"
#include <iostream>
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

bool Partition::verify_sizes_lemma3(vector<uint> const& L, uint l[3]) const
{
	// verify that neither a nor b is bigger than two thirds of the total and c is no bigger than L(l1) + L(l2) + max{0, 2(l2-l1-1)}
	cout << "verifying partition sizes\n";
	uint a_verts = a.size();
	uint b_verts = b.size();
	uint c_verts = c.size();
	uint n  = a_verts + b_verts + c_verts;

	return (a_verts <= 2*n/3) && 
		(b_verts <= 2*n/3) && 
		(c_verts <= 2*sqrt(2)*sqrt(n));

}

bool Partition::verify_sizes() const
{
	// verify that neither a nor b is bigger than two thirds of the total and c is no bigger than 2*sqrt(2)*sqrt(n)
	cout << "verifying partition sizes\n";
	uint a_verts = a.size();
	uint b_verts = b.size();
	uint c_verts = c.size();
	uint n  = a_verts + b_verts + c_verts;

	return (a_verts <= 2*n/3) && 
		(b_verts <= 2*n/3) && 
		(c_verts <= 2*sqrt(2)*sqrt(n));
}


void Partition::print() const
{
		cout << "Partition\n"; 
		cout << "  size of A: " << a.size() << '\n';
		cout << "  size of B: " << b.size() << '\n';
		cout << "  size of C: " << c.size() << '\n'; 
		cout << "  A = "; for( auto& v : a ) cout << v << ' '; cout << '\n';
		cout << "  B = "; for( auto& v : b ) cout << v << ' '; cout << '\n'; 
		cout << "  C = "; for( auto& v : c ) cout << v << ' '; cout << '\n';
}

uint Partition::total_num_verts() const
{
		return a.size() + b.size() + c.size();
}
