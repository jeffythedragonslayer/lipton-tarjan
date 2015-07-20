#include "stdafx.h"
#include "Lsr_Tri.h"
#include "Lsr.h"
#include "BaryPt.h"
#include "Tri.h"
using namespace std;

bool segcast_tri(BaryPt& bary, real &t, Seg3CR pq, Tri3CR tri)
{
        Vec3 n = tri.raw_normal(); // compute tri normal. Can be precalculated or cached if intersecting multiple segments against same tri
        real d = -pq.dir().dot(n); // compute denominator

        if( d <= 0 ) return false;   // segment is parallel to or pts away from tri

        Vec3 ap = pq.a - tri.a; // compute intersection t val of pq with plane of tri.  segment intersects iff 0 <= t <= 1.  Delay dividing by d until intersection has been found to pierce tri
        t = ap.dot(n);
        if( t < 0 || t > d ) return false;

        Vec3 e = -pq.dir().cross(ap); // compute barycentric coord components and test if within bounds
    
        if( (bary.v = tri.ac().dot(e)) < 0 || bary.v          > d ) return false;
        if( (bary.w = tri.ba().dot(e)) < 0 || bary.v + bary.w > d ) return false;

        t      /= d; // segment intersects tri. Perform delayed division and compute last barycentric coord component
        bary.v /= d;
        bary.w /= d;
        bary.u  = 1 - bary.v - bary.w;
        return true;
}

bool segcast_tri(real &t, Seg3CR pq, Tri3CR tri)
{
        Vec3 n = tri.raw_normal(); // compute tri normal. Can be precalculated or cached if intersecting multiple segments against same tri
        real d = -pq.dir().dot(n); // compute denominator

        if( d <= 0 ) return false;   // segment is parallel to or pts away from tri

        Vec3 ap = pq.a - tri.a; // compute intersection t val of pq with plane of tri.  segment intersects iff 0 <= t <= 1.  Delay dividing by d until intersection has been found to pierce tri
        t = ap.dot(n);
        if( t < 0 || t > d ) return false;

        Vec3 e = -pq.dir().cross(ap); // compute barycentric coord components and test if within bounds    
        real v = tri.ac().dot(e); if( v < 0 || v     > d ) return false;
        real w = tri.ba().dot(e); if( w < 0 || v + w > d ) return false;
        t /= d; // segment intersects tri. Perform delayed division
        return true;
}

bool linecast_tri(BaryPt& bary, Ray3CR pq, Tri3CR tri)
{
        Vec3 pc = tri.c - pq.pt;     
        Vec3 m  = pq.dir.cross(pc); // test if pq is in edges bc, ca and ab by testing that signed tetrahedral volumes, computed using real triple products, are all positive
        Vec3 pb = tri.b - pq.pt; if( (bary.u =  pb.dot(m)) < 0 ) return false; // box(pq, pc, pb);
        Vec3 pa = tri.a - pq.pt; if( (bary.v = -pa.dot(m)) < 0 ) return false; // box(pq, pa, pc);
        if( (bary.w = pq.dir.box(pb, pa)) < 0 ) return false;
        bary.normalize();
        return true;
}

bool linecast_tri(Ray3CR pq, Tri3CR tri)
{
        Vec3 pc = tri.c - pq.pt;     
        Vec3 m  = pq.dir.cross(pc); // test if pq is in edges bc, ca and ab by testing that signed tetrahedral volumes, computed using real triple products, are all positive
        Vec3 pb = tri.b - pq.pt; if(  pb.dot(m) < 0 ) return false; // box(pq, pc, pb);
        Vec3 pa = tri.a - pq.pt; if( -pa.dot(m) < 0 ) return false; // box(pq, pa, pc);
        return pq.dir.box(pb, pa) >= 0;
}

bool raycast_tri(BaryPt& bary, real& t, Ray3CR pq, Tri3CR tri)
{
        Vec3 n  = tri.raw_normal(); // normal can be precalculated or cached if intersecting multiple segments against same tri
        real d  = -pq.dir.dot(n); if( d <= 0 )              return false; // if denominator <= 0, ray is parallel to or pts away from tri, so exit early
        Vec3 ap =  pq.pt - tri.a; if( (t = ap.dot(n)) < 0 ) return false; // compute intersection t val of pq with plane of tri. A ray intersects iff 0 <= t.  Delay dividing by d until intersection has been found to pierce tri
        Vec3 e  = -pq.dir.cross(ap); // compute barycentric coord components and test if within bounds
        if( (bary.v = tri.ac().dot(e)) < 0 || bary.v          > d ) return false;
        if( (bary.w = tri.ba().dot(e)) < 0 || bary.v + bary.w > d ) return false;
        t      /= d; // ray intersects tri
        bary.v /= d;
        bary.w /= d;
        bary.u  = 1 - bary.v - bary.w;
        return true;
}

bool raycast_tri(real& t, Ray3CR ray, Tri3CR tri) // based on Moller, Tomas and Trumbore, "Fast, Minimum Storage Ray-Triangle Intersection", Journal of Graphics Tools, vol. 2, false. 1, pp 21-28, 1997
{
        Vec3   ab = tri.ab();
        Vec3   ac = tri.ac();
        Vec3   p  = ray.dir.cross(ac);
        Vec3   s  = ray.pt - tri.a;
        Vec3   q  = s.cross(ab);
               t  = ac.dot(q);
        real u  = s.dot(p);
        real v  = ray.dir.dot(q);
        real d  = ab.dot(p);
        if(   d > 0 ){if( u < 0 || u > d || v < 0 || u + v > d || t < 0 ) return false;} // tri front
        elif( d < 0 ){if( u > 0 || u < d || v > 0 || u + v < d || t > 0 ) return false;} // tri back
        else return false; // parallel ray, I removed a tolerance from this, was if( abs(d) <= Ep ) return false;
        t /= d; // u / d and v / d are barycentric cooridinates of intersection
        return true;
}