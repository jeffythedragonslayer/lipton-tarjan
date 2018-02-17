#pragma once
#include "typedefs.h"

struct BFSVert
{
        BFSVert() : parent(Graph::null_vertex()), level(0), descendant_cost(0) {}

        vertex_t parent;
        uint     level, descendant_cost;
};

