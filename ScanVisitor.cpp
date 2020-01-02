#include "ScanVisitor.h"
#include "graphutil.h"
#include <iostream>
using namespace std;
using namespace boost;

ScanVisitor::ScanVisitor(map<vertex_t, bool>* table, Graph* g, vertex_t x, uint l0) : table(table), g(g), x(x), l0(l0) {}

void ScanVisitor::found_edge(vertex_t current_node, edge_t e)
{
		auto v = source(e, *g);
		auto w = target(e, *g);
		cout << "foundedge " << v << ", " << w << endl;
		assert(edge(v, w, *g).second); // edge exists
		if( current_node != v ) swap(v, w);
		assert(current_node == v);
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

void ScanVisitor::scan_nonsubtree_edges_clockwise(vertex_t current_node, Graph const& g, Embedding const& embedding, BFSVisitorData const& bfs)
{
		auto v_it = bfs.verts.find(current_node);
		assert(v_it != bfs.verts.end());
		if( v_it->second.level > l0 ) return;

		auto em = embedding[current_node];
		for( auto e : em ){ 
				auto src = source(e, g);
				auto tar = target(e, g);
				if( src == tar ){
					cout << "ignoring circular node\n";
					continue;//throw FoundCircularNode(src);
				}

				if( nullptr == src || tar == nullptr || !edge(src, tar, g).second ){
					cout << "ignoring bad edge: " << src << ", " << tar << "\n";
					continue;
				}

				if( !bfs.is_tree_edge(e) ){
						found_edge(current_node, e);
						continue;
				}
				if( src != current_node ) swap(src, tar);
				assert(src == current_node); 
				auto tar_it = bfs.verts.find(tar);
				if( tar_it->second.level > l0 ) found_edge(current_node, e);
		}
		cout << "looking for children of " << current_node << '\n';
		auto vvv   = bfs.children.find(current_node);
		if( vvv == bfs.children.end() ) return; // no children
		cout << "from " << current_node << " looking for children\n";
		for( auto& c : vvv->second ){
				cout << "child: " << c << '\n'; 
				scan_nonsubtree_edges_clockwise(c, g, embedding, bfs);
		}
}