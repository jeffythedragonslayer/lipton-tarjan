#pragma once
#include "Vec.h"
#include <vector>
struct Matrix33;

struct AABBcr // center-radius axis aligned bounding box
{
        Pt3  c;
        Vec3 r;

                      AABBcr       () {};
                      AABBcr       (Pt3CR c, Vec3CR r);

        static AABBcr create       (std::vector<Pt3> const&);
        static AABBcr create       (AABBcrCR, Matrix33CR rot, Vec3CR W);
        static AABBcr merge        (AABBcrCR, AABBcrCR);

        Pt3           support_pt   (Vec3CR)                                                   const;

        bool          contains     (Pt3CR)                                                    const;
        bool          contains     (AABBcrCR)                                                 const;

        bool          test         (AABBcrCR)                                                 const;
        bool          test2        (AABBcrCR)                                                 const;
        bool          sweep_AABBcr (real& t0, real& t1, Vec3CR avel, AABBcrCR b, Vec3CR bvel) const;

        void          merge        (Pt3CR);

        Pt3           min_pt       ()                                                         const;
        Pt3           max_pt       ()                                                         const;
        Pt3           corner       (uint n)                                                   const;

        Pt3           closest_pt_to(Pt3CR)                                                    const;
        real          sqr_dist_to  (Pt3CR)                                                    const;

        real          volume       ()                                                         const;
        real          surface_area ()                                                         const;
};

struct AABBmm // min-max axis aligned bounding box
{
        Pt3  min, max;


                      AABBmm       ();
                      AABBmm       (Pt3CR min, Pt3CR max);
        static AABBmm create       (std::vector<Pt3> const&);
        static AABBmm merge        (AABBmmCR, AABBmmCR);
        
        Pt3           support_pt   (Vec3CR)   const;

        bool          contains     (Pt3CR)    const;
        bool          contains     (AABBmmCR) const;
        bool          test         (AABBmmCR) const;
        void          merge        (Pt3CR);

        Pt3           center       ()         const;
        Vec3          radius       ()         const;
        Pt3           corner       (uint n)   const;

        Pt3           closest_pt_to(Pt3CR)    const;
        real          sqr_dist_to  (Pt3CR)    const;

        real          volume       ()         const;
        real          surface_area ()         const;

        bool          operator==   (AABBmmCR) const;
};

AABBcr create_AABBcr(AABBmmCR);
AABBmm create_AABBmm(AABBcrCR);

struct Rect2u
{
        Rect2u() {};
        Rect2u(uint, uint, uint, uint);

        uint left, top, right, bottom;

        Rect2u& set(uint, uint, uint, uint);
        bool    contains(Pt2uCR);
};