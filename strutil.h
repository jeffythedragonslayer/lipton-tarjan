#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "lipton-tarjan.h"

std::ostream& operator<<(std::ostream& o, VertDesc v);
std::string to_string(EdgeDesc e, Graph const& g);
void print_cycle(std::vector<VertDesc> const& cycle);
void print_graph(Graph const& g);
void print_edges(Graph const& g);
