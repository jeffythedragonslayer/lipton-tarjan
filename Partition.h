#include <vector>
#include "typedefs.h"

struct Partition
{
        std::set<vertex_t> a, b, c;

	void print() const;
	void get_most_costly_part(std::set<vertex_t>** most_costly,
				  std::set<vertex_t>** other1,
				  std::set<vertex_t>** other2); 

}; 

