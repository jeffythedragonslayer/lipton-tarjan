#include "stdafx.h"
#include "Capsule.h"
using namespace std;
#define IT Capsule

bool IT::test(CapsuleCR c) const
{
        real s, t;
        return seg.closest_pt_to_seg(c.seg, s, t, Pt3(), Pt3()) <= sqr(r + c.r);
} 

Pt3 IT::support_pt(Vec3CR v) const {return v.normal() * r + (v.dot(seg.dir()) > 0 ? seg.a : seg.b);}