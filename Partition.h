#pragma once
#include <vector>
#include "typedefs.h"

struct Partition
{
		std::set<vertex_t> a, b, c;

		uint total_num_verts() const;

		bool verify_sizes_lemma3(std::vector<uint> const& L, uint l1, uint l2) const;
		bool verify_sizes(GraphCR g) const;
		bool verify_edges(GraphCR g) const;
		void print() const;
		void get_most_costly_part(std::set<vertex_t> const** most_costly,
									std::set<vertex_t> const** other1,
									std::set<vertex_t> const** other2) const; 
}; 