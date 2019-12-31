#include "Vert2UintMap.h"
#include <iostream>
using namespace std;

Vert2UintMap::Vert2UintMap(Graph const& g)
{
        VertIter vi, vi_end;
        uint i = 0;
        for( tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi ){
                vert2uint[*vi] = i;
                uint2vert[i] = *vi;
		vu_bimap.insert({*vi, i});
                ++i;
        }
}

void Vert2UintMap::print() const
{
       cout << "Vert2UintMap::print()    this == " << this << '\n';
       cout << "  vert2uint\n";
       for( auto a : vert2uint ){
               cout << " (" << a.first << ' ' << a.second << ")\n";
       }
       cout << '\n';
       cout << "  uint2vert\n";
       for( auto a : uint2vert ){
               cout << " (" << a.first << ' ' << a.second << ")\n";
       }
       cout << '\n';
       cout << "  vu_bimap\n";
       for( auto a : vu_bimap ){
               cout << " (" << a.left << ' ' << a.right << ")\n";
       }
       cout << '\n';
}