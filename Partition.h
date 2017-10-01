#include <vector>
#include "typedefs.h"

struct Partition
{
        std::vector<VertDesc> a, b, c;

	void print() const;
	void get_most_costly_part(std::vector<VertDesc>** most_costly,
				  std::vector<VertDesc>** other1,
				  std::vector<VertDesc>** other2); 

}; 

