#include "stdafx.h"
#include "Tri.h"
#include "BaryPt.h"
#include <algorithm>
using namespace std;

       Tri2::Tri2          ()                                                                                       {}
       Tri2::Tri2          (Pt2CR a, Pt2CR b, Pt2CR c)                            : a(a),      b(b),      c(c)      {}
       Tri2::Tri2          (real ax, real ay, real bx, real by, real cx, real cy) : a(ax, ay), b(bx, by), c(cx, cy) {}
real   Tri2::area          () const {return abs(signed_area());}
real   Tri2::perimeter     () const {return ab().length() + bc().length() + ca().length();}
real   Tri2::signed_area   () const {return ((a.x - c.x) * (b.y - c.y) - (a.y - c.y) * (b.x - c.x)) / 2;}
Vec2   Tri2::ab            () const {return b - a;}
Vec2   Tri2::bc            () const {return c - b;}
Vec2   Tri2::ca            () const {return a - c;}
Vec2   Tri2::ba            () const {return a - b;}
Vec2   Tri2::cb            () const {return b - c;}
Vec2   Tri2::ac            () const {return c - a;}
Pt2    Tri2::midpoint_ab   () const {return (a + b)/2;}
Pt2    Tri2::midpoint_bc   () const {return (b + c)/2;}
Pt2    Tri2::midpoint_ca   () const {return (c + a)/2;}
Tri2   Tri2::medial_tri    () const {return Tri2(midpoint_ab(), midpoint_bc(), midpoint_ca());}
       Tri2::operator Pt2* ()       {return &a;}
       Tri2::operator Pt2CP() const {return &a;}

bool   Tri2::contains      (Pt2CR p) const {auto b = barycentric(p); return b.v >= 0 && b.w >= 0 && (b.v + b.w) <= 1;}
bool   Tri2::is_real       ()        const {return a.is_real() && b.is_real();}
BaryPt Tri2::barycentric   (Pt2CR p) const {fail; return BaryPt();}
Pt2    Tri2::centroid      ()        const {return (a + b + c)/3;}
real   Tri2::inradius      ()        const {return 2*area() / perimeter();}
bool   Tri2::degenerate    ()        const {return a == b || b == c || a == c;}

real orient2d(Pt2CR a, Pt2CR b, Pt2CR c)
{
        Vec2 ca = a - c;
        Vec2 cb = b - c;
        return Matrix22(ca.x, ca.y,
                        cb.x, cb.y).det();
}

       Tri3::Tri3()                                                                                                                              {}
       Tri3::Tri3(Pt3CR a, Pt3CR b, Pt3CR c)                                                       : a(a),          b(b),          c(c)          {}
       Tri3::Tri3(real ax, real ay, real az, real bx, real by, real bz, real cx, real cy, real cz) : a(ax, ay, az), b(bx, by, bz), c(cx, cy, cz) {}
Vec3   Tri3::raw_normal      () const {return (b-a).cross(c-a);}
Vec3   Tri3::normal          () const {return raw_normal().normal();}
real   Tri3::area            () const {return raw_normal().length() / 2;}
real   Tri3::perimeter       () const {return ab().length() + bc().length() + ca().length();}
Vec3   Tri3::ab              () const {return b - a;}
Vec3   Tri3::bc              () const {return c - b;}
Vec3   Tri3::ca              () const {return a - c;}
Vec3   Tri3::ba              () const {return a - b;}
Vec3   Tri3::cb              () const {return b - c;}
Vec3   Tri3::ac              () const {return c - a;}
Pt3    Tri3::midpoint_ab     () const {return (a + b)/2;}
Pt3    Tri3::midpoint_bc     () const {return (b + c)/2;}
Pt3    Tri3::midpoint_ca     () const {return (c + a)/2;}
Tri3   Tri3::medial_tri      () const {return Tri3(midpoint_ab(), midpoint_bc(), midpoint_ca());}
       Tri3::operator Pt3*   ()       {return &a;}
       Tri3::operator Pt3CP  () const {return &a;}
Tri3&  Tri3::flip            ()       {swap(b, c); return self;}

BaryPt Tri3::barycentric(Pt3CR p) const
{
        Vec3   m  = raw_normal();
        Vec3   mn = m.abs(); // absolute components for determining proj plane
        real nu, nv, d; // nominators and denominator for u and v ratios

        // REFACTOR these using determinate predicate compute areas in plane of largest proj
        if(   mn.x >= mn.y && mn.x >= mn.z ) nu = Tri2(p.y, p.z, b.y, b.z, c.y, c.z).area(), nv = Tri2(p.y, p.z, c.y, c.z, a.y, a.z).area(), d =  m.x; // x is largest, project to yz plane 1/(2*area of ABC in yz plane)
        elif( mn.y >= mn.x && mn.y >= mn.z ) nu = Tri2(p.x, p.z, b.x, b.z, c.x, c.z).area(), nv = Tri2(p.x, p.z, c.x, c.z, a.x, a.z).area(), d = -m.y; // y is largest, project to xz plane
        else                                 nu = Tri2(p.x, p.y, b.x, b.y, c.x, c.y).area(), nv = Tri2(p.x, p.y, c.x, c.y, a.x, a.y).area(), d =  m.z; // z is largest, project to xy plane
        real u = 2 * nu / d;
        real v = 2 * nv / d;
        return BaryPt(u, v, 1 - u - v);
}

Pt3  Tri3::centroid() const {return (a + b + c)/3;}
real Tri3::inradius() const {return 2*area() / perimeter();}

bool Tri3::contains(Pt3CR p) const {auto b = barycentric(p); return b.v >= 0 && b.w >= 0 && (b.v + b.w) <= 1;} // this version of contains is broken, probably because barycentric hasn't been refactored

bool Tri3::contains2(Pt3CR p) const // Fauerby's algorithm, no idea how this works
{
        Vec3 ab_ = ab();
        Vec3 ac_ = ac();
        real a   = ab_.length_sqr();
        real b   = ab_.dot(ac_);
        real c   = ac_.length_sqr();
        real g   = a*c - b*b;
        Vec3 vp  = p - self.a;
        real d   = vp.dot(ab_);
        real e   = vp.dot(ac_);
        real x   = d*c - e*b;
        real y   = e*a - d*b;
        real z   = x + y - g;
        return z < 0 && x >= 0 && y >= 0;
}

bool Tri3::contains3(Pt3CR p) const
{
        Vec3 n = (c-a).cross(b-a); // compute tri normal
        // if cross product pts in same direction as normal pt is in edge (it is zero if is on edge)
        return (p-a).cross(b-a).dot(n) >= 0 && // compute cross products of vector from base of each edge to pt with each edge vector
               (p-b).cross(c-b).dot(n) >= 0 &&
               (p-c).cross(a-c).dot(n) >= 0; // if pt in all of edges it is in
}

bool Tri3::is_real   () const {return a.is_real() && b.is_real() && c.is_real();}
bool Tri3::degenerate() const {return a == b || b == c || a == c;}

Pt3 Tri3::closest_pt_to(Pt3CR p) const
{
        Vec3 ab_ = ab(); // check if p in vert region outside A
        Vec3 ac_ = ac();
        real d[7];
      
        Vec3 ap    = p - a; d[1] = ab_.dot(ap); d[2] = ac_.dot(ap); if( d[1] <= 0 &&  d[2]         <= 0                          ) return a;                                                         // bary(1,   0,   0)
        Vec3 bp    = p - b; d[3] = ab_.dot(bp); d[4] = ac_.dot(bp); if( d[3] >= 0 &&  d[4]         <= d[3]                       ) return b;                                                         // bary(0,   1,   0) check if p in vert region outside B
        real vc    = d[1]*d[4] - d[3]*d[2];                         if( vc   <= 0 &&  d[1]         >= 0    &&  d[3]         <= 0 ) return a + ab_ * d[1] / (d[1] - d[3]);                            // bary(1-v, v,   0) check if p in edge region of AB, if so return proj of P onto AB
        Vec3 cp    = p - c; d[5] = ab_.dot(cp); d[6] = ac_.dot(cp); if( d[6] >= 0 &&  d[5]         <= d[6]                       ) return c;                                                         // bary(0,   0,   1) check if p in vert region outside C
        real vb    = d[5]*d[2] - d[1]*d[6];                         if( vb   <= 0 &&  d[2]         >= 0    &&  d[6]         <= 0 ) return a + ac_ * d[2] / (d[2] - d[6]);                            // bary(1-w, 0,   w) check if p in edge region of AC, if so return proj of P onto AC
        real va    = d[3]*d[6] - d[5]*d[4];                         if( va   <= 0 && (d[4] - d[3]) >= 0    && (d[5] - d[6]) >= 0 ) return b + (c - b) * (d[4] - d[3]) / (d[4] - d[3] + d[5] - d[6]); // bary(0,   1-w, w) check if p in edge region of BC, if so return proj of P onto BC
        real denom = 1 / (va + vb + vc); // p in face region. Compute Q through its bary(u,v,w)
        real v     = vb * denom;
        real w     = vc * denom;
        return a + ab_ * v + ac_ * w; // = u*a + v*b + w*c, u = va * denom = 1 - v - w
}

real Tri3::sqr_dist_to(Pt3CR p) const {return closest_pt_to(p).sqr_dist_to(p);} // TODO implement this correctly

// The final test of algorithm is based on Shen, Heng, and Tang, "A Fast Triangle-Triangle Overlap Test Using Signed Distances", Journal of Graphics Tools, vol. 8      , false. 1, pp 17-23, 2003 and Guigue and Devillers, "Fast and Robust Triangle-Triangle Overlap Test Using Orientation Predicates", Journal of Graphics Tools, vol. 8, false. 1, pp 25-32, 2003
// The final test could be considered an edge-edge separating plane test with 9 possible cases narrowed down to only two pairs of edges that can actaully result in a seperation
// broken
bool test(Tri3CR A, Tri3CR B)
{
        Vec3 N1 = A.raw_normal();

        deny(N1.is_zero());

        Vec3 BDist(N1.dot(B.a - A.a), // test pts of B against plane of A
                   N1.dot(B.b - A.a),
                   N1.dot(B.c - A.a));
#if 0
        bool BDistIsZero = EP > BDist.abs(); // Ensure robustness with co-planar tris by zeroing small dists
        BDist = XMVectorSelect(BDist, VEC3_ZERO, BDistIsZero);

        bool BDistIsLess    = 0 > BDist;
        bool BDistIsGreater = BDist > 0;

        if( BDistIsLess || BDistIsGreater ) return false; // If all pts are on same side we don't intersect

        Vec3 N2 = B.raw_normal();

        deny(N2.is_zero());

        Vec3 ADist(N2.dot(A.a - B.a), // test pts of A against plane of B
                   N2.dot(A.b - B.a),
                   N2.dot(A.c - B.a));

        bool ADistIsZero = EP > abs(BDist); // ensure robustness with co-planar tris by zeroing small dists
        ADist = XMVectorSelect(ADist, VEC3_ZERO, ADistIsZero);

        bool ADistIsLess    = ADist < 0;
        bool ADistIsGreater = ADist > 0;
         
        if( ADistIsLess || ADistIsGreater ) return false; // if all pts are on same side we don't intersect

        if( ADistIsZero || BDistIsZero ){ // special case for co-planar tris
        Vec3 Axis = N1.cross(A.b - A.a); // compute an axis perpindicular to edge (pts out)
                real d = Axis.dot(A.a);

                real MinDist = min(B.a.dot(Axis), B.b.dot(Axis), B.c.dot(Axis)); // test pts of B against axis
                if( MinDist >= d ) return false;
                Axis = N1.cross(A.c - A.b); d = Axis.dot(A.b); MinDist = min(B.a.dot(Axis), B.b.dot(Axis), B.c.dot(Axis)); if( XMVector4GreaterOrEqual(MinDist.dot(d))) return false; // Edge (A.b, A.c)
                Axis = N1.cross(A.a - A.c); d = Axis.dot(A.c); MinDist = min(B.a.dot(Axis), B.b.dot(Axis), B.c.dot(Axis)); if( XMVector4GreaterOrEqual(MinDist.dot(d))) return false; // Edge (A.c, A.a)
                Axis = N2.cross(B.b - B.a); d = Axis.dot(B.a); MinDist = min(A.a.dot(Axis), A.b.dot(Axis), A.c.dot(Axis)); if( XMVector4GreaterOrEqual(MinDist.dot(d))) return false; // Edge (B.a, B.b)
                Axis = N2.cross(B.c - B.b); d = Axis.dot(B.b); MinDist = min(A.a.dot(Axis), A.b.dot(Axis), A.c.dot(Axis)); if( XMVector4GreaterOrEqual(MinDist.dot(d))) return false; // Edge (B.b, B.c)
                Axis = N2.cross(B.a - B.c); d = Axis.dot(B.c); MinDist = min(A.a.dot(Axis), A.b.dot(Axis), A.c.dot(Axis)); if( XMVector4GreaterOrEqual(MinDist.dot(d))) return false; // Edge (B.c,B.a)
                return true;
        }

        // find single vertex of A and B (ie vertex on opposite side of plane from other two) and reorder edges so we can compute signed edge/edge dists
        // if ( (V0 >= 0 && V1 <  0 && V2 <  0) || (V0 >  0 && V1 <= 0 && V2 <= 0) || (V0 <= 0 && V1 >  0 && V2 >  0) || (V0 <  0 && V1 >= 0 && V2 >= 0) ) then V0 is singular;
        // If our singular vertex is not on positive side of plane we reverse tri winding so that overlap comparisons will compare correct edges with correct signs
        bool ADistIsLessEqual    = ADistIsLess    || ADistIsZero;
        bool ADistIsGreaterEqual = ADistIsGreater || ADistIsZero;

        Vec3 AA0, AA1, AA2;
        bool bPositiveA;

        if(   ADistIsGreaterEqual  || ADistIsLess    ) || (ADistIsGreater, ADistIsLessEqual,    Select0111)) AA0 = A.a, AA1 = A.b, AA2 = A.c, bPositiveA = true;  // A.a singular, from + to -
        elif( ADistIsLessEqual     || ADistIsGreater ) || (ADistIsLess,    ADistIsGreaterEqual, Select0111)) AA0 = A.a, AA1 = A.c, AA2 = A.b, bPositiveA = false; // A.a singular, from - to +
        elif( ADistIsGreaterEqual  || ADistIsLess    ) || (ADistIsGreater, ADistIsLessEqual,    Select1011)) AA0 = A.b, AA1 = A.c, AA2 = A.a, bPositiveA = true;  // A.b singular, from + to -
        elif( ADistIsLessEqual     || ADistIsGreater ) || (ADistIsLess,    ADistIsGreaterEqual, Select1011)) AA0 = A.b, AA1 = A.a, AA2 = A.c, bPositiveA = false; // A.b singular, from - to +
        elif( ADistIsGreaterEqual  || ADistIsLess    ) || (ADistIsGreater, ADistIsLessEqual,    Select1101)) AA0 = A.c, AA1 = A.a, AA2 = A.b, bPositiveA = true;  // A.c singular, from + to -
        elif( ADistIsLessEqual     || ADistIsGreater ) || (ADistIsLess,    ADistIsGreaterEqual, Select1101)) AA0 = A.c, AA1 = A.b, AA2 = A.a, bPositiveA = false; // A.c singular, from - to +
        else {fail; return false;}

        bool BDistIsLessEqual    = BDistIsLess    || BDistIsZero;
        bool BDistIsGreaterEqual = BDistIsGreater || BDistIsZero;

        Vec3 BB0, BB1, BB2;
        bool bPositiveB;

        if(   XMVectorSelect(BDistIsGreaterEqual, BDistIsLess,    Select0111) || XMVectorSelect(BDistIsGreater, BDistIsLessEqual,    Select0111))) BB0 = B.a, BB1 = B.b, BB2 = B.c, bPositiveB = true;  // B.a is singular, from + to -
        elif( XMVectorSelect(BDistIsLessEqual,    BDistIsGreater, Select0111) || XMVectorSelect(BDistIsLess,    BDistIsGreaterEqual, Select0111))) BB0 = B.a, BB1 = B.c, BB2 = B.b, bPositiveB = false; // B.a is singular, from - to +
        elif( XMVectorSelect(BDistIsGreaterEqual, BDistIsLess,    Select1011) || XMVectorSelect(BDistIsGreater, BDistIsLessEqual,    Select1011))) BB0 = B.b, BB1 = B.c, BB2 = B.a, bPositiveB = true;  // B.b is singular, from + to -
        elif( XMVectorSelect(BDistIsLessEqual,    BDistIsGreater, Select1011) || XMVectorSelect(BDistIsLess,    BDistIsGreaterEqual, Select1011))) BB0 = B.b, BB1 = B.a, BB2 = B.c, bPositiveB = false; // B.b is singular, from - to +
        elif( XMVectorSelect(BDistIsGreaterEqual, BDistIsLess,    Select1101) || XMVectorSelect(BDistIsGreater, BDistIsLessEqual,    Select1101))) BB0 = B.c, BB1 = B.a, BB2 = B.b, bPositiveB = true;  // B.c is singular, from + to -
        elif( XMVectorSelect(BDistIsLessEqual,    BDistIsGreater, Select1101) || XMVectorSelect(BDistIsLess,    BDistIsGreaterEqual, Select1101))) BB0 = B.c, BB1 = B.b, BB2 = B.a, bPositiveB = false; // B.c is singular, from - to +
        else {fail; return false;}

        Vec3 Delta0, Delta1;// reverse direction of test depending on whether singular vertices are same sgn or different signs
        if( bPositiveA ^ bPositiveB ) Delta0 = BB0 - AA0; Delta1 = AA0 - BB0;
        else                          Delta0 = AA0 - BB0, Delta1 = BB0 - AA0;
        if( (BB2 - BB0).cross(AA2 - AA0).dot(Delta0) > 0 ) return false; // check if tris overlap on line of intersection between planes of two tris by finding signed line dists
        if( (BB1 - BB0).cross(AA1 - AA0).dot(Delta1) > 0 ) return false;
#endif
        return true;
}
