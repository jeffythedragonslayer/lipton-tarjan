#include <vector>
#include <memory>

struct Edge;

struct Vertex
{
        Edge* incident;
};

struct Edge
{
        Vertex* src;
        Vertex* tar;
        Edge*   c1;
        Edge*   cc1;
        Edge*   c2;
        Edge*   cc2;
};

struct Graph
{
        std::vector<Vertex*> vertices;
        std::vector<Edge*>   edges;

        void create_vertex(Vertex*);
        void delete_vertex(Vertex*);
        void create_edge(Edge*);
        void delete_edge(Edge*);
};
