#pragma once
#include "style.h"
#undef self
#include <boost/graph/adjacency_list.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::property<boost::vertex_index_t, int>> bgraph;

struct Partition
{
	bgraph a, b;
	bgraph c;
};

Partition lipton_tarjan(bgraph g);
