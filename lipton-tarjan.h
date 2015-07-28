#pragma once
#include "style.h"
#include <vector>
#include <list>
#include <utility>

typedef int Graph;

struct Partition
{
	Graph a, b;
	Graph c;
};

Partition lipton_tarjan(Graph g);
