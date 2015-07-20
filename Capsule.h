#pragma once
#include "Lsr.h"

struct Capsule 
{
        Seg3 seg;
        real r;

             Capsule    (Seg3CR seg,       real r) : seg(seg),  r(r) {}
             Capsule    (Pt3CR a, Pt3CR b, real r) : seg(a, b), r(r) {}

        bool test       (CapsuleCR) const;
        Pt3  support_pt (Vec3CR)    const;
};