#pragma once
#include "typedefs.h"

struct EmbedStruct
{
        EmbeddingStorage storage;
        Embedding        em;
        Graph const*     g;

        EmbedStruct(Graph const*); 

        bool test_planar() const;
        void print() const;
};
