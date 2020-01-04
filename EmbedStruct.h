#pragma once
#include "typedefs.h"

struct EmbedStruct
{
        EmbeddingStorage storage;
        Embedding        em;
        Graph const*     g;

        EmbedStruct() = default;
        EmbedStruct(Graph const*); 

        bool test_planar() const;
        void print() const;
};

bool test_planar_workaround(Embedding&, Graph* g);
EmbedStruct ctor_workaround(Graph*);