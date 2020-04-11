//=======================================================================
// Copyright 2015 - 2020 Jeff Linahan
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#pragma once
#include "typedefs.h"

struct BFSVert
{
        BFSVert() : parent(Graph::null_vertex()), level(0), descendant_cost(0) {}

        vertex_t parent;
        uint     level, descendant_cost;
};