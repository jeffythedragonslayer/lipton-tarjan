#include "stdafx.h"
#include "MatrixNM_Plane.h"
#include "MatrixNM.h"
#include "Plane.h"

Matrix34 reflection_matrix(PlaneCR p) // plane must be normalized
{
        return Matrix34(-2*p.n.x*p.n.x + 1, -2*p.n.y*p.n.x,     -2*p.n.z*p.n.x,
                        -2*p.n.x*p.n.y,     -2*p.n.y*p.n.y + 1, -2*p.n.z*p.n.y,
                        -2*p.n.x*p.n.z,     -2*p.n.y*p.n.z,     -2*p.n.z*p.n.z + 1,
                         2*p.n.x*p.d,        2*p.n.y*p.d,        2*p.n.z*p.d);
}

real dot(PlaneCR, Vec4CR);
real dot(Vec4CR, PlaneCR);

Matrix44 shadow_matrix(Vec4CR l, PlaneCR p)
{
        real d = Vec4(p).dot(l); // is this dot coord?
        return Matrix44(p.n.x*l.x + d,  p.n.x*l.y,      p.n.x*l.z,         p.n.x*l.w,
                        p.n.y*l.x,      p.n.y*l.y + d,  p.n.y*l.z,         p.n.y*l.w,
                        p.n.z*l.x,      p.n.z*l.y,      p.n.z*l.z + d,     p.n.z*l.w,
                       -p.d  *l.x,     -p.d  *l.y,     -p.d  *l.z,     d - p.d  *l.w);
}