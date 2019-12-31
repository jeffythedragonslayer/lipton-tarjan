#pragma once
#include "Partition.h"
#include "Vert2UintMap.h"
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/property_map.hpp>
#include <vector>
#define STLALL(x) (x).begin(), (x).end()

std::tuple<Partition, Vert2UintMap, Vert2UintMap> lipton_tarjan(Graph const& g_orig);

struct NotPlanarException {}; 
