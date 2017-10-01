#pragma once
#include "typedefs.h"

struct BFSVert
{
        BFSVert() : parent(Graph::null_vertex()), level(0), descendant_cost(0) {}

        VertDesc parent;
        uint     level, descendant_cost;
};

