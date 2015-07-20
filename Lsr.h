#pragma once
#include "Vec.h"

struct Seg2
{
        Pt2 a, b;

             Seg2      ()                              {}
             Seg2      (Pt2CR a, Pt2CR b) : a(a), b(b) {} 
        Vec2 dir       ()                           const;
        real length_sqr()                           const;
        real length    ()                           const;
        Pt2  eval_t    (real t)                     const;
        Pt2  midpoint  ()                           const; 
        bool test      (real& t, Pt2& p, Seg2CR s2) const;
};

struct Seg3
{
        Pt3 a, b;

             Seg3             () {}
             Seg3             (Pt3CR a, Pt3CR b) : a(a), b(b) {}
        Vec3 dir              ()                                              const;
        real length_sqr       ()                                              const;
        real length           ()                                              const;
        Pt3  eval_at          (real t)                                        const;
        Pt3  midpoint         ()                                              const; 
        Pt3  closest_pt_to    (Pt3CR c, real& t)                              const;
        real sqr_dist_to      (Pt3CR)                                         const;
        real closest_pt_to_seg(Seg3CR s2, real& s, real& t, Pt3& c1, Pt3& c2) const;
};

Pt3 ptOnLineSegmentNearestpt(Seg3CR, Pt3CR);

struct Ray2
{
        Pt2  pt;
        Vec2 dir;

             Ray2    () {}
             Ray2    (Pt2CR, Vec2CR);
        Pt2  b       ()       const;
        Pt2  eval_at (real t) const;
        Ray2 inverse ()       const;
};

struct Ray3
{
        Pt3  pt;
        Vec3 dir;

             Ray3                   () {}
             Ray3                   (Pt3CR, Vec3CR);
        Pt3  b                      ()                                              const;
        Pt3  eval_at                (real)                                          const;
        bool collinear              (Pt3CR p)                                       const;
        Pt3  LINE_closest_pt_to     (real& t, Pt3CR c)                              const;
        real LINE_sqr_dist_to       (Pt3CR)                                         const;
        real LINE_closest_pt_to_line(Ray3CR s2, real& s, real& t, Pt3& c1, Pt3& c2) const;
        Ray3 inverse                ()                                              const;
};