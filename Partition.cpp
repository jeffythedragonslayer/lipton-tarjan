#include "Partition.h"
#include <iostream>
using namespace std;

void Partition::get_most_costly_part(vector<vertex_t>** most_costly,
				     vector<vertex_t>** other1,
				     vector<vertex_t>** other2)
{
	if( a.size() > b.size() && a.size() > c.size() ){
		*most_costly = &a;
		*other1      = &b;
		*other2      = &c;
		return;
	}
	if( b.size() > a.size() && b.size() > c.size() ){
		*most_costly = &b;
		*other1      = &a;
		*other2      = &c;
		return;
	}
	if( c.size() > a.size() && c.size() > b.size() ){
		*most_costly = &c;
		*other1      = &a;
		*other2      = &b;
		return;
	}
	assert(0);
}

void Partition::print() const
{
	cout << "<<<< Partition >>>>\n"; 
	cout << "sizes of A, B, C: " << a.size() << ", " << b.size() << ", " << c.size() << '\n';
	cout <<   "A = "; for( auto& v : a ) cout << v << ' ';
	cout << "\nB = "; for( auto& v : b ) cout << v << ' ';
	cout << "\nC = "; for( auto& v : c ) cout << v << ' '; 
	cout << "<<<< /Partition >>>>\n";
}
