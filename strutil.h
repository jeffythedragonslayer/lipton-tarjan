#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "lipton-tarjan.h"

//std::ostream& operator<<(std::ostream& o, vertex_t v);
std::string   to_string(edge_t e, Graph const& g, Vert2UintMap& vmap);
void          print_cycle(std::vector<vertex_t> const& cycle);
void          print_graph_special(Graph const& g, Vert2UintMap& vmap);
void          print_graph2(Graph const& g);
void          print_edges(Graph const& g, Vert2UintMap& vmap);
