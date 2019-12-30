#include <vector>
#include "Vert2UintMap.h"
#include "typedefs.h"

struct Partition
{
		std::set<vertex_t> a, b, c;

		uint total_num_verts() const;

		void print(Vert2UintMap const& vmap) const;
		void get_most_costly_part(std::set<vertex_t>** most_costly,
									std::set<vertex_t>** other1,
									std::set<vertex_t>** other2); 

}; 

