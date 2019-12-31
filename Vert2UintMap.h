#pragma once
#include "typedefs.h"
#include <boost/bimap.hpp>

// induces a canonical numbering on graph vertices
struct Vert2UintMap
{ 
		std::map<vertex_t, uint>     vert2uint;
		std::map<uint, vertex_t>     uint2vert;
		boost::bimap<vertex_t, uint> vu_bimap; 

		Vert2UintMap() = default;
		Vert2UintMap(Graph const&);
		void print() const;
};