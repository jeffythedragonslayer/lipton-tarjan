#pragma once
#include "Partition.h"
#include "typedefs.h"

Partition theorem4_connected(GraphCR g, std::vector<uint> const& L, uint l[3], uint r, Graph* g_shrunk);
Partition theorem4(GraphCR g, boost::associative_property_map<vertex_map> const& vertid_to_component, std::vector<uint> const& num_verts_per_component, Partition const&);
Partition theorem4_disconnected(GraphCR, uint n, uint num_components, boost::associative_property_map<vertex_map> const& vertid_to_component, std::vector<uint> const& num_verts_per_component, Partition const&);