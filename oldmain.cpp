diff --git a/main.cpp b/main.cpp
index 36299a7..c67fbdb 100644
--- a/main.cpp
+++ b/main.cpp
@@ -29,13 +29,6 @@ int main(int argc, char* argv[])
 
                 uint n = num_vertices(g);
 
-                auto prop_map = get(vertex_index, g); // writing to this property map has side effects in the graph
-                VertIter vi, vend;
-                uint i = 0;
-                for( tie(vi, vend) = vertices(g); vi != vend; ++vi ){
-                        prop_map[*vi] = i;
-                        ++i;
-                }
 
                 cout << "n: " << n << '\n';
                 uint e = num_edges(g);
