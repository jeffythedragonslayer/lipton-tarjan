#pragma once
#include "lipton-tarjan.h" 
#include <boost/bimap.hpp>

struct Vert2UintMap
{
	std::map<vertex_t, uint>     vert2uint;
	std::map<uint, vertex_t>     uint2vert;
	boost::bimap<vertex_t, uint> vu_bimap; 
};

extern Vert2UintMap vmap;
