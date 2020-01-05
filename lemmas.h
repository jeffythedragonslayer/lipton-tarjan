#pragma once
#include "Partition.h"
#include "BFSVisitorData.h"
#include <iostream>
#include <vector>

Partition lemma3(GraphCR g_orig, std::vector<uint> const& L, uint l1, uint l2, uint r, BFSVisitorData const& vis_data_orig, BFSVisitorData const& vis_data, std::vector<vertex_t> const& cycle);