#include <vector>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/property_map.hpp>
typedef boost::adjacency_list<boost::listS, boost::listS, boost::undirectedS, boost::property<boost::vertex_index_t, int>, boost::property<boost::edge_index_t, int>> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor 														      VertDesc;

struct Partition
{
        std::vector<VertDesc> a, b, c;

	void print() const;
	void get_most_costly_part(std::vector<VertDesc>** most_costly,
				  std::vector<VertDesc>** other1,
				  std::vector<VertDesc>** other2); 

}; 

