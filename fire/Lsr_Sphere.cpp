#include "stdafx.h"
#include "Lsr_Sphere.h"
#include "Sphere.h"
#include "Lsr.h"
using namespace std;

bool sweep_unitsphere_against_pt(real& t, Ray3CR ray, Pt3CR p)
{
        Vec3 base_to_p = ray.pt - p;
        return lower_quadratic_root(t,
                                    ray.dir.length_sqr(),
                                    2 * ray.dir.dot(base_to_p),
                                    base_to_p.length_sqr() - 1) && t > 0;
}

bool segcast_sphere(real& t, Pt3& q, Seg3CR seg, SphereCR s)
{
        Vec3 m = seg.a - s.c;
        real b = m.dot(seg.dir());
        real c = m.length_sqr() - s.r*s.r;
        if( c > 0 && b > 0 ) return false; // exit if r’s origin out s (c > 0) and r pointing away from s (b > 0)

        real a     = seg.length_sqr();
        real discr = b*b - a*c;
        if( discr < 0 ) return false; // negative discriminant corresponds to ray missing sphere
  
        t = (-b - sqrt(discr)) / a; // ray now found to intersect sphere, compute min t val of intersection
        if( t >= 1 ) return false;  // ray intersects sphere but segment does not

        if( t <  0 ) t = 0;         // if t is negative, ray started in sphere so clamp t to zero
        q = seg.eval_at(t);
        return true;
}

bool segcast_sphere(Seg3CR seg, SphereCR s)  {return seg.sqr_dist_to(s.c) <= s.r*s.r;}

bool sweep_unitsphere_against_seg(real& t, Pt3& q, Ray3CR ray, Seg3CR seg)
{
        Vec3   base_to_a        = seg.a - ray.pt;
        real edge_dot_vel       = seg.dir().dot(ray.dir);
        real edge_dot_base_to_a = seg.dir().dot(base_to_a);
        real edge_lsq           = seg.length_sqr();
        real a                  = edge_lsq * -ray.dir.length_sqr() + sqr(edge_dot_vel);
        real b                  = edge_lsq * (2 * ray.dir.dot(base_to_a)) - 2 * edge_dot_vel * edge_dot_base_to_a;
        real c                  = edge_lsq * (1 - base_to_a.length_sqr()) + sqr(edge_dot_base_to_a);
        if( lower_quadratic_root(t, a, b, c) && t >= 0 && t <= 1 ){ // does swept sphere collide against infinite edge
                real f = (edge_dot_vel * t - edge_dot_base_to_a) / edge_lsq;
                if( f >= 0 && f <= 1 ) return q = seg.eval_at(f), true;// is intersection within line segment
        }
        return false;
}

bool raycast_sphere(Ray3CR ray, SphereCR s)
{
        Vec3 m = ray.pt - s.c;
        real c = m.length_sqr() - s.r*s.r; if( c <= 0 ) return true;  // ray origin in sphere
        real b = m.dot(ray.dir);           if( b >  0 ) return false; // ray pointing away from sphere
        return b*b - c >= 0;
}

bool raycast_sphere(real& t, Pt3& q, Ray3CR ray, SphereCR s)
{
        Vec3 m = ray.pt - s.c;
        real b = m.dot(ray.dir);
        real c = m.length_sqr() - s.r*s.r;

        if( c > 0 && b > 0 ) return false; // exit if r’s origin out s (c > 0) and r pointing away from s (b > 0)

        real discr = b*b - c; if( discr < 0 ) return false;
    
        t = -b - sqrt(discr); // ray now found to intersect sphere, compute min t val of intersection
        if( t < 0 ) t = 0;    // if t is negative, ray started in sphere

        q = ray.eval_at(t);
        return true;
}

bool linecast_sphere(Ray3CR line, SphereCR s) {return line.LINE_sqr_dist_to(s.c) <= sqr(s.r);}

bool raycast_sphere2(real& t, Ray3CR ray, SphereCR sphere)
{
        Vec3 l     = sphere.c - ray.pt;
        real s     = l.dot(ray.dir); // s is proj of l onto ray direction
        real l2    = l.length_sqr();
        real r2    = sphere.r*sphere.r;
        real m2    = l2 - s*s;
        bool NoInt = (s < 0 && l2 > r2) || m2 > r2; // If ray origin is out sphere and center of sphere is behind ray origin there is false intersection
        // if sqr dist center of sphere to proj is greater than radius sqr ray will miss sphere
        real q  = sqrt(r2-m2); // The ray hits sphere, compute nearest intersection pt
        real t1 = s - q;
        real t2 = s + q;
        t = l2 <= r2 ? t1 : t2;
        return !NoInt;
}
