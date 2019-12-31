#pragma once
#include "Partition.h"
#include "Vert2UintMap.h"
#include "BFSVisitorData.h"
#include <iostream>

Partition lemma3_cllmax(GraphCR g_orig, uint l[3], uint r, BFSVisitorData const& vis_data_orig, BFSVisitorData const& vis_data, Vert2UintMap& vmap, Vert2UintMap& vmap_shrunk, std::vector<vertex_t> const& cycle);
