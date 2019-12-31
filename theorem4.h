#pragma once
#include "Partition.h"
#include "typedefs.h"

Partition theorem4(GraphCR g, boost::associative_property_map<vertex_map> const& vertid_to_component, std::vector<uint> const& num_verts_per_component);