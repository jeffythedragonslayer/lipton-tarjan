#include "stdafx.h"
#include "Sphere.h"
#include "AABB.h"
#include "Lsr_Sphere.h"
#include "MatrixNM.h"
#include "Lsr.h"
#include "Tri.h"
#include "d3dUtil.h"
#include "Vec.h"
#include "MatrixNM_Vec.h"
#include <algorithm>
#include <vector>
using namespace std;
#define IT Circle

     IT::IT            ()                                                            {}
     IT::IT            (Pt2CR c, real r)  : c(c),           r(r)                     {assert(c.is_real() && r >= 0);}
     IT::IT            (Pt2CR a)          : c(a),           r(EP)                    {}
     IT::IT            (Pt2CR a, Pt2CR b) : c((a + b) / 2), r(a.dist_to(b) / 2 + EP) {}
real IT::area          ()        const                {return PI * r * r;}
real IT::circumference ()        const                {return TAU * r;}
bool IT::contains      (Pt2CR p) const                {return c.sqr_dist_to(p) <= r*r;}

real in_circle2d(Pt2CR a, Pt2CR b, Pt2CR c, Pt2CR d)
{
        Vec2 da = a - d;
        Vec2 db = b - d;
        Vec2 dc = c - d;
        return Matrix33(da.x, da.y, da.length_sqr(),
                        db.x, db.y, db.length_sqr(),
                        dc.x, dc.y, dc.length_sqr()).det();
}

void QuadraticCircleFit(vector<Pt2>, Circle) {fail;}

bool CircleFit(vector<Pt2> pts, uint maxIters, Circle& cir, bool initialCenterIsmean)
{
        Pt2 mean = pts[0];
        for( uint i0 = 1; i0 < pts.size(); ++i0 ) mean += pts[i0];
	    real invnum_pts = 1./pts.size();
	    mean *= invnum_pts; 
	    if( initialCenterIsmean ) cir.c = mean; // The initial guess for the center
	    else QuadraticCircleFit(pts, cir); 
	    int i1;
	    for( i1 = 0; i1 < maxIters; ++i1 ){
		        Pt2 current = cir.c; // update the iterates
		        real lenmean = 0; // compute mean L, dL/da, dL/db
		        Vec2 derLenmean = VEC2_ZERO;
		        for( uint i0 = 0; i0 < pts.size(); ++i0 ){
			            Vec2 diff = pts[i0] - cir.c;
			            real length = diff.length();
			            if( length > EP ){
				                lenmean    += length;
				                derLenmean -= diff / length;
			            }
		        }
		           lenmean *= invnum_pts;
		        derLenmean *= invnum_pts;
                        cir = Circle(mean + lenmean*derLenmean, lenmean);
		        Vec2 diff = cir.c - current;
		        if( abs(diff[0]) <= EP && abs(diff[1]) <= EP ) break;
	    }
	    return i1 < maxIters;
}

#define IT Sphere
        
     IT::IT                ()                                                            {}
     IT::IT                (Pt3CR c, real r)  : c(c),           r(r)                     {assert(r >= 0);}
     IT::IT                (Pt3CR a)          : c(a),           r(EP)                    {}
     IT::IT                (Pt3CR a, Pt3CR b) : c((a + b) / 2), r(a.dist_to(b) / 2 + EP) {}
real IT::sqr_dist_to_center(Pt3CR p)    const                                            {return c.sqr_dist_to(p);}
bool IT::contains          (Pt3CR p)    const                                            {return sqr_dist_to_center(p) <= r*r;}
Pt3  IT::support_pt        (Vec3CR dir) const                                            {return c + dir.normal() * r;}
 
#if 0
Sphere::Sphere(Pt3CR O, Pt3CR A, Pt3CR B)
{
        Vec3 a = A - O;
        Vec3 b = B - O;

        real Denominator = 2 * a.cross(b).length_sqr();

        Vec3 o = ((b^2) * a.cross(b).cross(a) +
                  (a^2) * b.cross(a.cross(b))) / Denominator;

        r = Vec3::N(o) + EP;
        c = O + o;
}

Sphere::Sphere(Pt3CR O, Pt3CR A, Pt3CR B, Pt3CR C)
{
        Vec3 a = A - O;
        Vec3 b = B - O;
        Vec3 c = C - O;

        real Denominator = 2 * create_matrix33_from_rows(a, b, c).det()

        Vec3 o = ((c^2) * a.cross(b) +
                  (b^2) * c.cross(a) +
                  (a^2) * b.cross(c)) / Denominator;

        r = Vec3::N(o) + EP;
        c = O + o;
}
#endif

Sphere IT::merge(SphereCR a, SphereCR b)
{
        Vec3 d = b.c - a.c;
        real l = d.length_sqr();
        if( sqrt(b.r - a.r) >= l ) return b.r >= a.r ? b : a; // sphere with larger radius encloses other; return larger 
        l = sqrt(l); // spheres partially overlapping or disjoint
        Sphere s(a.c, (l + a.r + b.r) / 2);
        if( l > EP ) s.c += ((s.r - a.r) / l) * d;
        return s;
}

Sphere IT::bad_create(vector<Pt3> const& pts)
{
        auto aabb        = AABBcr::create(pts);
        real max_dis_sqr = 0;
        for( auto pt : pts ) max_dis_sqr = max(max_dis_sqr, pt.sqr_dist_to(aabb.c));
        return Sphere(aabb.c, sqrt(max_dis_sqr));
}

Sphere IT::ritter_create(vector<Pt3> const& pts)
{
        Pt3 minX, minY, minZ, maxX, maxY, maxZ; maxX = maxY = maxZ = -(minX = minY = minZ = VEC3_INF);
        for( auto pt : pts ){
                if( pt.x < minX.x ) minX = pt; if( pt.x > maxX.x ) maxX = pt;
                if( pt.y < minY.y ) minY = pt; if( pt.y > maxY.y ) maxY = pt;
                if( pt.z < minZ.z ) minZ = pt; if( pt.z > maxZ.z ) maxZ = pt;

        }
        Vec3 dist(maxX.sqr_dist_to(minX), // use min/max pair farthest apart to form initial sphere
                  maxY.sqr_dist_to(minY),
                  maxZ.sqr_dist_to(minZ));
        Sphere s;
        if( dist.x > dist.y ){
                if( dist.x > dist.z ) s.c = (maxX + minX) / 2, s.r = sqrt(dist.x) / 2;
                else                  s.c = (maxZ + minZ) / 2, s.r = sqrt(dist.z) / 2;
        } else {
                if( dist.y > dist.z ) s.c = (maxY + minY) / 2, s.r = sqrt(dist.y) / 2;
                else                  s.c = (maxZ + minZ) / 2, s.r = sqrt(dist.z) / 2;
        }
        for( auto pt : pts ){  // add any pts not in sphere
                Vec3 delta = pt - s.c;
                real dist2 = delta.length_sqr();
                if( dist2 > s.r*s.r ){
                    real dist = sqrt(dist2);
                         s.r  = (s.r + dist) / 2; // adjust sphere to include new pt
                         s.c += (1 - s.r/dist) * delta;
                }
        }
        return s;
}

#if 0
Sphere welzl_create(Pt3CP pts, uint num_pts, Pt3 sos[], uint num_sos)
{
        if( !num_pts ) switch( num_sos ){
                case 0: return Sphere(VEC3_ZERO, 0);
                case 1: return Sphere(sos[0]);
                case 2: return Sphere(sos[0], sos[1]);
                case 3: return Sphere(sos[0], sos[1], sos[2]);
                case 4: return Sphere(sos[0], sos[1], sos[2], sos[3]);
        }
        auto ball = welzl_create(pts, --num_pts, sos, num_sos);
        if( ball.contains(pts[num_pts]) ) return ball;
        sos[num_sos] = pts[num_pts];
        return welzl_create(pts, num_pts, sos, num_sos + 1);
}

Sphere IT::welzel_create(std::vector<Pt3> const& pts) {return welzl_create(&pts[0], pts.size(), nullptr, 0);}
#endif

bool IT::test(SphereCR b) const {return sqr_dist_to_center(b.c) <= sqr(r + b.r);}

void IT::merge(Pt3CR p)
{
        Vec3 d     = p - c;
        real dist2 = d.length_sqr();
        if( dist2 > r*r ){ // only update sphere if pt p is out it
                real dist    = sqrt(dist2);
                real new_rad = (r + dist) / 2;
                real k       = (new_rad - r) / dist;
                     r       = new_rad;
                     c      += d * k;
        }
}

bool IT::sweep(real& t, Vec3CR avel, SphereCR b, Vec3CR bvel) const
{
        Vec3 v = avel - bvel; // subtract movement of bvel from both avel and bvel, making b stationary
        Pt3  q; // can now test directed segment s = a.c + tv, v = (v0-v1)/||v0-v1|| against expanded sphere for intersection
        real vlen = v.length();
        return raycast_sphere(t, q, Ray3(c, v / vlen), Sphere(b.c, b.r + r)) ? t <= vlen : false;
}

real IT::volume()       const {return 2/3. * TAU * r*r*r;}
real IT::surface_area() const {return 2    * TAU * r*r;}

real in_sphere(Pt3CR a, Pt3CR b, Pt3CR c, Pt3CR d, Pt3CR e)
{
        Vec3 ea = a - e;
        Vec3 eb = b - e;
        Vec3 ec = c - e;
        Vec3 ed = d - e;
        return Matrix44(ea.x, ea.y, ea.z, ea.length_sqr(),
                        eb.x, eb.y, eb.z, eb.length_sqr(),
                        ec.x, ec.y, ec.z, ec.length_sqr(),
                        ed.x, ed.y, ed.z, ed.length_sqr()).det();
}

void build_geo_sphere(vector<Pt3>& verts, IndexList16& inds, uint subdivs, real rad)
{
        assert(verts.empty() && inds.empty());
        Pt3 pos[] = {Pt3(-1,   0,   PHI), Pt3( 1,   0,   PHI), Pt3(-1,   0,  -PHI), Pt3( 1,   0,  -PHI),
                     Pt3( 0,   PHI,   1), Pt3( 0,   PHI,  -1), Pt3( 0,  -PHI,   1), Pt3( 0,  -PHI,  -1),
                     Pt3( PHI,   1,   0), Pt3(-PHI,   1,   0), Pt3( PHI,  -1,   0), Pt3(-PHI,  -1,   0)};

		uint k[] = {0, 6, 1,     0, 1, 4,    0, 4, 9,    0, 9, 11,    0, 11, 6,
                    6, 10, 1,    1, 8, 4,    4, 5, 9,    9, 2, 11,    11, 7, 6,
		            10, 8, 1,    8, 5, 4,    5, 2, 9,    2, 7, 11,    7, 10, 6,
	                10, 3, 8,    8, 3, 5,    5, 3, 2,    2, 3, 7,     7, 3, 10};

        for( auto& p : pos ) verts.emplace_back(p);
        for( auto& i : k   ) inds.emplace_back(i);
        DO_TIMES(i, subdivs) subdivide(verts, inds);
        weld_duplicates(verts, inds);
        for( auto& v : verts ) v.set_length(rad); // project verts onto sphere and scale
}

void QuadraticSphereFit(vector<Pt3>, Sphere)
{
        fail;
}

bool sphere_fit(Sphere& sphere, vector<Pt3> pts, uint maxIters, bool initialCenterIsmean)
{
        Pt3 mean = pts[0]; // compute mean of the data points
        uint i0;
        for( i0 = 1; i0 < pts.size(); ++i0 ) mean += pts[i0];
        real invnum_pts = 1./pts.size();
        mean *= invnum_pts;

        if( initialCenterIsmean ) sphere.c = mean; // initial guess for the center
        else QuadraticSphereFit(pts, sphere);

        uint i1;
        for( i1 = 0; i1 < maxIters; ++i1 ){
                Pt3  curr       = sphere.c; // update the iterates
                real lenmean    = 0; // compute mean L, dL/da, dL/db, dL/dc
                Vec3 derLenmean = VEC3_ZERO;
                for( Pt3CR p : pts ){
                        Vec3 diff   = p - sphere.c;
                        real length = diff.length();
                        if( length > EP ){
                                lenmean += length;
                                real invLength = 1./length;
                                derLenmean -= invLength*diff;
                        }
                }
                lenmean    *= invnum_pts;
                derLenmean *= invnum_pts;
                sphere = Sphere(mean + lenmean*derLenmean, lenmean);
                Vec3 diff = sphere.c - curr;
                if( abs(diff[0]) <= EP && abs(diff[1]) <= EP && abs(diff[2]) <= EP ) break;
        }
        return i1 < maxIters;
}