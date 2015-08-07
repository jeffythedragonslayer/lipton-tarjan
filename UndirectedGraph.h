#include <vector>

typedef unsigned int uint;

struct UndirectedGraph
{
                          UndirectedGraph(uint size);
        void              add_edge       (uint a, uint b);
        void              delete_edge    (uint a, uint b);
        bool              edge_exists    (uint a, uint b) const;

        uint              num_verts      ()               const;
        uint              num_edges      ()               const;
        std::vector<uint> get_neighbors  (uint n) const;

        void              clear          ();
        bool              empty          () const;

        std::vector<uint> bfs_tree       () const;

private:
        std::vector<std::vector<uint>> adj;
};

std::vector<uint> get_vertex_levels (std::vector<uint> const&);
std::vector<uint> num_verts_in_level(std::vector<uint> const&);
