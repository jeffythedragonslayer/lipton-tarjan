#include "UndirectedGraph.h"
#include <algorithm>
#include <vector>
#include <queue>
#include <cassert>
using namespace std;

UndirectedGraph::UndirectedGraph(uint size) {adj.resize(size);}

void UndirectedGraph::add_edge(uint a, uint b)
{
        assert(a < num_verts() && b < num_verts());
        if( edge_exists(a, b) ) return;

        adj[a].push_back(b);
        adj[b].push_back(a);
}

void UndirectedGraph::delete_edge(uint a, uint b)
{
        if( !edge_exists(a, b) ) return;

        auto& adj_a = adj[a]; auto a_pos = find(adj_a.begin(), adj_a.end(), b); adj_a.erase(a_pos);
        auto& adj_b = adj[b]; auto b_pos = find(adj_b.begin(), adj_b.end(), a); adj_b.erase(b_pos);
}

bool UndirectedGraph::edge_exists(uint a, uint b) const
{ 
        auto& adj_a = adj[a];
        auto& adj_b = adj[b];
        return find(adj_a.begin(), adj_a.end(), b) != adj_a.end() &&
               find(adj_b.begin(), adj_b.end(), a) != adj_b.end();
}

void         UndirectedGraph::clear        ()             {adj.clear();} 
bool         UndirectedGraph::empty        ()       const {return adj.empty();}
uint         UndirectedGraph::num_verts    ()       const {return adj.size();}
vector<uint> UndirectedGraph::get_neighbors(uint n) const {return adj[n];}

uint UndirectedGraph::num_edges() const
{
        uint size = 0;
        for( auto& v : adj ) size += v.size();
        return size / 2;
}

vector<uint> UndirectedGraph::breadth_first_spanning_tree() const
{
        vector<bool> seen   (false, num_verts());
        vector<uint> parents(num_verts()); // map vert id -> parent id
        queue<uint> q;

        q.push(0); // setup root node
        seen   [0] = true;
        parents[0] = 0;

        while( !q.empty() ){
                auto top = q.front(); q.pop();
                for( auto& a : adj[top] ) if( !seen[a] ){
                        seen[a] = true;
                        parents[a] = top;
                        q.push(a);
                }
        }

        return parents;
}
