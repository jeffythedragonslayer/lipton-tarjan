#include "ScanVisitor.h"
#include "graphutil.h"
#include <iostream>
using namespace std;
using namespace boost;

ScanVisitor::ScanVisitor(map<vertex_t, bool>* table, Graph* g, vertex_t x, uint l0) : table(table), g(g), x(x), l0(l0) {}

void ScanVisitor::foundedge(vertex_t V, edge_t e)
{
		auto v = source(e, *g);
		auto w = target(e, *g);
		if( V != v ) swap(v, w);
		assert(V == v);
		cout << "foundedge " << v << ", " << w;
		if ( !(*table)[w] ){
				(*table)[w] = true;
				assert(x != w); 
				cout << "   !!!!!!!going to add " << x << ", " << w;
				edges_to_add.insert(make_pair(x, w));
		}
		cout << "     going to delete " << v << ", " << w;
		edges_to_delete.insert(make_pair(v, w)); 
		cout << '\n';
}

void ScanVisitor::finish()
{
		cout << "finishing\n";
		cout << "edges to add size: " << edges_to_add.size() << '\n';
		cout << "adding: ";
		for( auto& p : edges_to_add    ){
				assert(p.first != p.second);
				cout << '(' << p.first << ", " << p.second << ")   ";
				add_edge(p.first, p.second, *g);
		}
		cout << '\n';
		cout << "edges to remove size: " << edges_to_delete.size() << '\n';
		cout << "removing: ";
		for( auto& p : edges_to_delete ){ 
				cout << '(' << p.first << ", " << p.second << ")   ";
				remove_edge(p.first, p.second, *g);
		}
		cout << '\n';
}

void ScanVisitor::scan_nonsubtree_edges(vertex_t v, Graph const& g, Embedding const& embedding, BFSVisitorData const& bfs)
{
		auto v_it = bfs.verts.find(v);
		assert(v_it != bfs.verts.end());
		if( v_it->second.level > l0 ) return;

		auto em = embedding[v];
		for( auto e : em ){ 
				auto src = source(e, g);
				auto tar = target(e, g);
				if( src == tar ) throw FoundCircularNode(src);

				if( !bfs.is_tree_edge(e) ){
						foundedge(v, e);
						continue;
				}
				if( src != v ) swap(src, tar);
				assert(src == v); 
				auto tar_it = bfs.verts.find(tar);
				if( tar_it->second.level > l0 ) foundedge(v, e);
		}
		cout << "looking for children of " << v << '\n';
		auto vvv   = bfs.children.find(v);
		if( vvv == bfs.children.end() ) return; // no children
		cout << "from " << v << " looking for children\n";
		for( auto& c : vvv->second ){
				cout << "child: " << c << '\n'; 
				scan_nonsubtree_edges(c, g, embedding, bfs);
		}
}