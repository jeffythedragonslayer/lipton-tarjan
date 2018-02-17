#include <vector>
#include "typedefs.h"

struct Partition
{
        std::vector<vertex_t> a, b, c;

	void print() const;
	void get_most_costly_part(std::vector<vertex_t>** most_costly,
				  std::vector<vertex_t>** other1,
				  std::vector<vertex_t>** other2); 

}; 

