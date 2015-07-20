#pragma once
struct BaryPt;
#include "Vec.h"

struct Tri2
{
        Pt2 a, b, c;

        operator Pt2*            ();
        operator Pt2CP           ()       const;

               Tri2              ();
               Tri2              (Pt2CR a, Pt2CR b, Pt2CR c);
               Tri2              (real ax, real ay, real bx, real by, real cx, real cy);

        real   signed_area       ()        const;
        real   area              ()        const;
        real   perimeter         ()        const;
        BaryPt barycentric       (Pt2CR)   const;
        Pt2    centroid          ()        const;
        Pt2    incenter          ()        const;
        real   inradius          ()        const;

        bool   contains          (Pt2CR)   const;
        bool   is_real           ()        const;

        Vec2   ab                ()        const;
        Vec2   bc                ()        const;
        Vec2   ca                ()        const;
        Vec2   ba                ()        const;
        Vec2   cb                ()        const;
        Vec2   ac                ()        const;
        Pt2    midpoint_ab       ()        const;
        Pt2    midpoint_bc       ()        const;
        Pt2    midpoint_ca       ()        const;

        Tri2   medial_tri        ()       const;
        Tri2   inner_napolean_tri()       const;
        Tri2   outer_napolean_tri()       const;
        bool   degenerate        ()       const;
};

real orient2d(Pt2 a, Pt2 b, Pt2 c); // det predicate, 2 * signed area of tri abc (positive iff counterclockwise)

struct Tri3
{
        Pt3 a, b, c;

        operator Pt3*              ();
        operator Pt3CP             ()        const;

               Tri3                ();
               Tri3                (Pt3CR a, Pt3CR b, Pt3CR c); 
               Tri3                (real ax, real ay, real az, real bx, real by, real bz, real cx, real cy, real cz);

        Vec3   raw_normal          ()      const;
        Vec3   normal              ()      const;

        real   area                ()      const;
        real   perimeter           ()      const;
        BaryPt barycentric         (Pt3CR) const;
        Pt3    centroid            ()      const;
        Pt3    incenter            ()      const;
        real   inradius            ()      const;
        bool   contains            (Pt3CR) const;
        bool   contains2           (Pt3CR) const;
        bool   contains3           (Pt3CR) const;

        bool   is_real             ()      const;

        Pt3    closest_pt_to       (Pt3CR) const;
        real   sqr_dist_to         (Pt3CR) const;

        Vec3   ab                  ()      const;
        Vec3   bc                  ()      const;
        Vec3   ca                  ()      const;
        Vec3   ba                  ()      const;
        Vec3   cb                  ()      const;
        Vec3   ac                  ()      const;

        Pt3    midpoint_ab         ()      const;
        Pt3    midpoint_bc         ()      const;
        Pt3    midpoint_ca         ()      const;

        Tri3   medial_tri          ()      const;
        Tri3   inner_napolean_tri  ()      const;
        Tri3   outer_napolean_tri  ()      const;

        Tri3&  flip                ();
        bool   degenerate          ()      const;
};