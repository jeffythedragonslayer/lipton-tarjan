#include "stdafx.h"
#include "Cylinder_Lsr.h"
#include "Lsr.h"
#include "Cylinder.h"
#include <cmath>
using namespace std;

bool segcast_cylinder(Seg3CR seg, CylinderCR cyl, real& t) // intersect segment S(t)=sa+t(sb-sa), 0<=t<=1 against cylinder specified by p, q and r
{
        Vec3  m  = seg.a - cyl.seg.a;
        real md = m.dot(cyl.seg.dir());
        real nd = seg.dir().dot(cyl.seg.dir());
        real dd = cyl.seg.length_sqr();

        // test if segment fully out either endcap of cylinder
        if( md < 0  && md + nd < 0  ) return false; // segment out ‘p’ side of cylinder
        if( md > dd && md + nd > dd ) return false; // segment out ‘q’ side of cylinder

        real nn = seg.length_sqr();
        real mn = m.dot(seg.dir());
        real a  = dd * nn - nd * nd;
        real k  = m.length_sqr() - cyl.r*cyl.r;
        real c  = dd * k - md * md;

        if( abs(a) < EP ){ // segment runs parallel to cylinder axis

            if( c > 0 ) return false; // ‘a’ and thus segment lie out cylinder
        
            // now known that segment intersects cylinder; figure out how it intersects
            t = md < 0 ? -mn / nn :       // intersect segment against ‘p’ endcap
                         ( md > dd ? (nd - mn) / nn : // intersect segment against ‘q’ endcap
                                     0);              // ‘a’ lies in cylinder
            return true;
        }
        real b     = dd * mn - nd * md;
        real discr = b * b - a * c;
        if( discr < 0 ) return false; // false real roots; false intersection

        real t0 = t = (-b - sqrt(discr)) / a;

        if( md + t * nd < 0 ){ // intersection out cylinder on ‘p’ side
                if( nd <= 0 ) return false; // segment pointing away from endcap
                t = -md / nd;
                if( k + t * (2 * mn + t * nn) <= 0 ) return true; // keep intersection if( S(t) - p).sqr_length() <= r^2
        } elif( md + t * nd > dd ){ // intersection out cylinder on ‘q’ side
                if( nd >= 0 ) return false; // segment pointing away from endcap
                t = (dd - md) / nd;
                if( k + dd - 2 * md + t * (2 * (mn - nd) + t * nn) <= 0 ) return true; // keep intersection if( S(t) - q).sqr_length() <= r^2
        }
        t = t0;
        return t >= 0 && t <= 1; // intersection if segment intersects cylinder between end-caps
}