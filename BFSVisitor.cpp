#include "BFSVisitor.h"
uisng namespace std;

vector<vertex_t> ancestors(vertex_t v) const
{
        //cout << "first v: " << v << '\n';
        //cout << "root: " << vis.root << '\n';
        vector<vertex_t> ans = {v};
        while( v != data.root ){
                auto v_it = data.verts.find(v);
                if( v_it == data.verts.end() ) return ans; // root is in a different connected component
                v = v_it->second.parent;
                //cout << "pushing back v: " << v << '\n';
                ans.push_back(v);
        }
        return ans;
}

vector<vertex_t> BFSVisitor::ancestors(vertex_t v) const
{
        //cout << "first v: " << v << '\n';
        //cout << "root: " << data.root << '\n';
        vector<vertex_t> ans = {v};
        while( v != data.root ){
                auto v_it = data.verts.find(v);
                if( v_it == data.verts.end() ) return ans; // root is in a different connected component
                v = v_it->second.parent;
                //cout << "pushing back v: " << v << '\n';
                ans.push_back(v);
        }
        return ans;
}