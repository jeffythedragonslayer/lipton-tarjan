#include "Vert2UintMap.h"
#include <iostream>
using namespace std;

Vert2UintMap vmap, vmap_copy;

void Vert2UintMap::print() const
{
       cout << "Vert2UintMap::print()    this == " << this << '\n';
       cout << "  vert2uint\n";
       for( auto a : vert2uint ){
               cout << "       (" << a.first << ' ' << a.second << ") ";
       }
       cout << '\n';
       cout << "  uint2vert\n";
       for( auto a : uint2vert ){
               cout << "       (" << a.first << ' ' << a.second << ") ";
       }
       cout << '\n';
       cout << "  vu_bimap\n";
       for( auto a : vu_bimap ){
               cout << "       (" << a.left << ' ' << a.right << ") ";
       }
       cout << '\n';
}
