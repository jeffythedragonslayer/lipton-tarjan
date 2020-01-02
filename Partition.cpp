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
