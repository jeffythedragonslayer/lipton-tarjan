#pragma once
#include "typedefs.h"

struct EmbedStruct
{
        EmbeddingStorage storage;
        Embedding        em;
        Graph*           g;

        EmbedStruct(Graph*); 

        bool test_planar(); 
        void print();
};
