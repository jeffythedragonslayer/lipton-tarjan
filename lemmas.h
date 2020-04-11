//=======================================================================
// Copyright 2015 - 2020 Jeff Linahan
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#pragma once
#include "Partition.h"
#include "BFSVisitorData.h"
#include <iostream>
#include <vector>

Partition lemma3(GraphCR g_orig, std::vector<uint> const& L, uint l1, uint l2, uint r, BFSVisitorData const& vis_data_orig, BFSVisitorData const& vis_data, std::vector<vertex_t> const& cycle, Graph* g_shrunk);