//=======================================================================
// Copyright 2015 - 2020 Jeff Linahan
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
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