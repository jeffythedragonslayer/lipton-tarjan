#include "stdafx.h"
#include "AABB.h"
#include "MatrixNM.h"
#include <algorithm>
using namespace std;
using namespace boost::algorithm;

AABBcr::AABBcr(Pt3CR c, Vec3CR r) : c(c), r(r) {assert(c.is_real() && r.abs() == r);}

Pt3 AABBcr::support_pt(Vec3CR dir) const
{
        return c + Pt3(r.x*sgn(dir.x),
                       r.y*sgn(dir.y),
                       r.z*sgn(dir.z));
}

bool AABBcr::test(AABBcrCR b) const
{
        return abs(c.x - b.c.x) <= r.x + b.r.x &&
               abs(c.y - b.c.y) <= r.y + b.r.y &&
               abs(c.z - b.c.z) <= r.z + b.r.z;
}

bool AABBcr::contains(Pt3CR pt) const
{
        Vec3 v = (pt - c).abs();
        return v.x <= r.x &&
               v.y <= r.y &&
               v.z <= r.z;
}

bool AABBcr::contains(AABBcrCR aabb) const {return contains(aabb.max_pt()) && contains(aabb.min_pt());}

AABBcr AABBcr::create(vector<Pt3> const& pt) {return create_AABBcr(AABBmm::create(pt));}

AABBcr AABBcr::create(AABBcrCR orig, Matrix33CR rot, Vec3CR W) // transform orig AABBcr by matrix m and translation t, find maximum extents
{
        Pt3  c = W;
        auto r = VEC3_ZERO;
        DO_TIMES(i, 3) DO_TIMES(j, 3){
                c[i] += orig.c[j] *     rot(i, j);
                r[i] += orig.r[j] * abs(rot(i, j));
        }
        return AABBcr(c, r);
}

AABBcr AABBcr::merge(AABBcrCR a, AABBcrCR b) {return create_AABBcr(AABBmm(a.min_pt().minimum(b.min_pt()), a.max_pt().maximum(b.max_pt())));}

Pt3 AABBcr::closest_pt_to(Pt3CR p) const {Pt3 q; DO_TIMES(i, 3) q[i] = clamp(p[i], c[i] - r[i], c[i] + r[i]); return q;}

real AABBcr::sqr_dist_to(Pt3CR p) const
{
        real sqr_dist = 0;
        DO_TIMES(i, 3){
                real d                = abs(p[i] - c[i]) - r[i];
                if( d > 0 ) sqr_dist += d*d;
        }
        return sqr_dist;
}

Pt3 AABBcr::corner(uint n) const
{
        return c + Vec3(n&1 ? r.x : -r.x,
                        n&2 ? r.y : -r.y,
                        n&4 ? r.z : -r.z);
}

bool AABBcr::sweep_AABBcr(real &t0, real &t1, Vec3CR avel, AABBcrCR b, Vec3CR bvel) const // intersect AABBs self and b moving with constant velocities avel and bvel. on intersection, return time of first and last contact in t0 and t1
{
        if( test(b) ) return t0 = t1 = 0, true;
        Vec3 v    = bvel - avel; // use relative velocity; effectively treating self as stationary
             t0   = 0; // initialize times of first and last contact
             t1   = 1;
        Pt3  amin =   min_pt();
        Pt3  amax =   max_pt();
        Pt3  bmin = b.min_pt();
        Pt3  bmax = b.max_pt();
        DO_TIMES(i, 3){
                if( v[i] < 0 ){
                        if( bmax[i] < amin[i] ) return false; // moving apart
                        if( amax[i] < bmin[i] ) max(t0, (amax[i] - bmin[i]) / v[i]);
                        if( bmax[i] > amin[i] ) min(t1, (amin[i] - bmax[i]) / v[i]);
                } elif( v[i] > 0 ){
                        if( bmin[i] > amax[i] ) return false; // moving apart
                        if( bmax[i] < amin[i] ) max(t0, (amin[i] - bmax[i]) / v[i]);
                        if( amax[i] > bmin[i] ) min(t1, (amax[i] - bmin[i]) / v[i]);
                }
                if( t0 > t1 ) return false;
        }
        return true;
}

void AABBcr::merge(Pt3CR p)
{
        Pt3 min = min_pt().minimum(p);
        Pt3 max = max_pt().maximum(p);
            c   = (max + min) / 2;
            r   = max - c;
}

Pt3  AABBcr::min_pt       () const {return c - r;}
Pt3  AABBcr::max_pt       () const {return c + r;}
real AABBcr::volume       () const {return 8 * r.x * r.y * r.z;}
real AABBcr::surface_area () const {return 8 * (r.x*r.y + r.y*r.z + r.x*r.z);}

bool AABBcr::test2(AABBcrCR B) const
{
        Pt3 MinA = c   - r;
        Pt3 MaxA = c   + r;
        Pt3 MinB = B.c - B.r;
        Pt3 MaxB = B.c + B.r;
        return MinA.x <= MaxB.x && MinA.y <= MaxB.y && MinA.z <= MaxB.z &&
               MinB.x <= MaxA.x && MinB.y <= MaxA.y && MinB.z <= MaxA.z;
}

AABBmm::AABBmm(Pt3CR min, Pt3CR max) : min(min), max(max) {assert(max.x >= min.x && max.y >= min.y && max.z >= min.z);}

AABBmm AABBmm::create(std::vector<Pt3> const& pt)
{
        Pt3 min =  VEC3_INF;
        Pt3 max = -VEC3_INF;
        for( auto& p : pt ){
                min.minimize(p);
                max.maximize(p);
        }
        return AABBmm(min, max);
}

AABBmm AABBmm::merge(AABBmmCR a, AABBmmCR b) {return AABBmm(a.min.minimum(b.min), a.max.maximum(b.max));}

Pt3  AABBmm::center() const {return (min + max) / 2;}
Vec3 AABBmm::radius() const {return (max - min) / 2;}

Pt3  AABBmm::support_pt(Vec3CR dir) const
{
        return Pt3(dir.x>0 ? max.x : min.x,
                   dir.y>0 ? max.y : min.y,
                   dir.z>0 ? max.z : min.z);
}

bool AABBmm::contains(Pt3CR pt) const
{
        return pt.x >= min.x && pt.x <= max.x &&
               pt.y >= min.y && pt.y <= max.y &&
               pt.z >= min.z && pt.z <= max.z;
}

bool AABBmm::contains(AABBmmCR b) const
{
        return b.min.x >= min.x && b.max.x <= max.x &&
               b.min.y >= min.y && b.max.y <= max.y &&
               b.min.z >= min.z && b.max.z <= max.z;
}

bool AABBmm::test(AABBmmCR b) const
{
        return min.x <= b.max.x && max.x >= b.min.x &&
               min.y <= b.max.y && max.y >= b.min.y &&
               min.z <= b.max.z && max.z >= b.min.z;
}

void AABBmm::merge(Pt3CR p) {min.maximize(p); max.minimize(p);}

Pt3 AABBmm::corner(uint n) const
{
        return Pt3(n&1 ? max.x : min.x,
                   n&2 ? max.y : min.y,
                   n&4 ? max.z : min.z);
}

Pt3 AABBmm::closest_pt_to(Pt3CR p) const {Pt3 q; DO_TIMES(i, 3) q[i] = clamp(p[i], min[i], max[i]); return q;}

real AABBmm::sqr_dist_to(Pt3CR p) const
{
        real d = 0;
        DO_TIMES(i, 3) d += sqr(p[i] > max[i] ?
                                p[i] - max[i] :
                               (p[i] < min[i] ? min[i] - p[i] : 0));
        return d;
}

real AABBmm::volume() const {return (max.x - min.x) * (max.y - min.y) * (max.z - min.z);}

bool AABBmm::operator==(AABBmmCR other) const {return self.min == other.min && self.max == other.max;}

AABBcr create_AABBcr(AABBmmCR aabb)
{
        Pt3 c = (aabb.max + aabb.min) / 2;
        return AABBcr(c, aabb.max - c);
}

AABBmm create_AABBmm(AABBcrCR aabb) // piece of crap waiting to explode
{
        uint roundmode = _controlfp(0, 0); _controlfp(_RC_DOWN,  _MCW_RC); // round away frojjm zero to be sure AABBmm is not smaller than AABBcr
        Pt3  min       = aabb.c - aabb.r;  _controlfp(_RC_UP,    _MCW_RC);
        Pt3  max       = aabb.c + aabb.r;  _controlfp(roundmode, _MCW_RC);
        return AABBmm(min, max);
}

Rect2u::Rect2u(uint left, uint top, uint right, uint bottom) : left(left), top(top), right(right), bottom(bottom) {} 

Rect2u& Rect2u::set(uint left, uint top, uint right, uint bottom) {self.left = left; self.top = top; self.right = right; self.bottom = bottom; return self;}

bool Rect2u::contains(Pt2uCR)
{
        fail;
        return false;
}