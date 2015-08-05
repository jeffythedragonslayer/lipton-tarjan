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

vector<uint> UndirectedGraph::bfs_tree() const
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

vector<uint> get_vertex_levels(vector<uint> const& bfs_tree)
{ 
        vector<uint> levels(bfs_tree.size(), 1);
        levels[0] = 0;
        for( uint i = 0; i < bfs_tree.size(); ++i ){
                uint v = bfs_tree[i]; 
                while( v ){
                        v = bfs_tree[v];
                        levels[i]++;
                } 
        }
        return levels;
}

vector<uint> num_verts_in_level(vector<uint> const& vert_levels)
{ 
        uint max_level = 0;
        for( auto l : vert_levels ) max_level = max(max_level, l);

        vector<uint> num_verts_in_level(max_level+1, 0);
        for( auto l : vert_levels ) num_verts_in_level[l]++; 
        return num_verts_in_level;
}
