#pragma once
#include "Partition.h"
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/property_map.hpp>
#include <vector>
#define STLALL(x) (x).begin(), (x).end()

Partition lipton_tarjan(Graph const& g_orig);

struct NotPlanarException {}; 
