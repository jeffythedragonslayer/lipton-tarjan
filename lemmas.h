#pragma once
#include "Partition.h"
#include "BFSVisitorData.h"
#include <iostream>

Partition lemma3(GraphCR g_orig, uint l[3], uint r, BFSVisitorData const& vis_data_orig, BFSVisitorData const& vis_data, std::vector<vertex_t> const& cycle);