#pragma once
#include "Vec.h"

struct Cone
{
        Pt3  c;
        Vec3 n;
        real r, h;

	     Cone           (Pt3CR c, Vec3CR d, real r);
	     Cone           (Pt3CR c, Vec3CR n, real r, real h);
        real side_length_sqr()            const;
        real side_length    ()            const;
        real volume         ()            const;
        real surface_area   ()            const;
        Pt3  solid_centroid ()            const;
        Pt3  hollow_centroid()            const;
        real sin_theta      ()            const; 
        Pt3  support_pt     (Vec3CR)      const;
};