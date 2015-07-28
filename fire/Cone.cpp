#include "stdafx.h"
#include "Cone.h"
using namespace std;
#define IT Cone

     IT::IT                 (Pt3CR c, Vec3CR d, real r)         : c(c), n(d.normal()), r(r), h(d.length()) {}
     IT::IT                 (Pt3CR c, Vec3CR n, real r, real h) : c(c), n(n),          r(r), h(h)          {}
real IT::side_length_sqr    () const {return r*r + h*h;}
real IT::side_length        () const {return sqrt(side_length_sqr());}
real IT::volume             () const {return PI * r*r * h / 3;}
real IT::surface_area       () const {return PI * r * (side_length() + r);}
Pt3  IT::solid_centroid     () const {return c + n * 3 / 4;}
Pt3  IT::hollow_centroid    () const {return c + n * 2 / 3;}
real IT::sin_theta          () const {return r / side_length();}

Pt3 IT::support_pt(Vec3CR v) const
{
        return c + (v.dot(n) / v.length() < sin_theta() ?
               r*(v - v.proj(n)).znormal()              :
	       n*h);
}