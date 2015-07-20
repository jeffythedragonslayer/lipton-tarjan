#include "stdafx.h"
#include "Sphere_Tri.h"
#include "Plane.h"
#include "Lsr_Sphere.h"
#include "Sphere.h"
#include "Plane_Tri.h"
#include "SSTPacket.h"
#include "Lsr.h"
#include <algorithm>
using namespace std;

bool   test(Pt3& p, SphereCR s, Tri3CR t) {return s.contains(p = t.closest_pt_to(s.c));}
bool pierce(Pt3& p, SphereCR s, Tri3CR t) {return (p = t.closest_pt_to(s.c)).sqr_dist_to(s.c) < sqr(s.r);}
bool pierce(        SphereCR s, Tri3CR t) {return      t                    .sqr_dist_to(s.c) < sqr(s.r);}

bool pts_and_edges(SSTPacket& pac) 
{
        pac.t    = 1; // pac.t and pac.q will decrease as earlier collisions are discovered
        bool hit = false;
        DO_TIMES(i, 3){
                real t;
                Pt3  q;
                if( sweep_unitsphere_against_pt (t,    pac.ray,                  pac.tri[i]       ) && t < pac.t ){hit = true; pac.q = pac.tri[i]; pac.t = t;}
                if( sweep_unitsphere_against_seg(t, q, pac.ray, Seg3(pac.tri[i], pac.tri[(i+1)%3])) && t < pac.t ){hit = true; pac.q = q;          pac.t = t;}
        }
        return hit;
}

bool calctimes(SSTPacket& pac)
{
        if( pac.tri_norm_dot_vel ){ // not parallel, sphere will intersect with tri plane, calculate plane intersection interval
                pac.embedded = false;
                pac.t0      = (-1-pac.signed_tri_plane_dist) / pac.tri_norm_dot_vel;
                pac.t1      = ( 1-pac.signed_tri_plane_dist) / pac.tri_norm_dot_vel;
                if( pac.t0 > pac.t1 ) swap(pac.t0, pac.t1);
                if( pac.t0 > 1 || pac.t1 < 0 ) return false; // plane collision already ended or hasn't begun yet
                pac.t0 = saturate(pac.t0);
                pac.t1 = saturate(pac.t1);
                return true;
        } elif( abs(pac.signed_tri_plane_dist) >= 1 ) return pac.embedded = false; // false, too far away from plane, false collision ever
        pac.embedded = true; // true, intersects plane at all times
        pac.t0       = 0;
        pac.t1       = 1;
        return true;
}

void sweep_unitsphere_against_tri_work(SSTPacket& pac)
{
        pac.tri_plane        = create_plane(pac.tri);
        pac.tri_norm_dot_vel = pac.tri_plane.n.dot(pac.ray.dir);
        if( pac.tri_norm_dot_vel > 0 ){
#ifdef SKIP_BACK_FACING
                pac.hit = false;
                return;
#else
                pac.tri.flip();
                pac.tri_plane        = create_plane(pac.tri); // recalculate for back facing tri
                pac.tri_norm_dot_vel = pac.tri_plane.n.dot(pac.ray.dir);
#endif
        }
        pac.signed_tri_plane_dist = pac.tri_plane.dot_coord(pac.ray.pt);
        if( !calctimes(pac) ){
                pac.hit = false;
                return;
        } elif( !pac.embedded ){
                pac.q = (pac.ray.pt - pac.tri_plane.n) + pac.t0 * pac.ray.dir;
                if( pac.tri.contains2(pac.q) ){ // sweep unitsphere against tri in
                        pac.t      = pac.t0;
                        pac.normal = pac.tri_plane.n;
                        pac.in     = pac.hit = true;
                        return;
                }
        }
        pac.in = false;
        if( pts_and_edges(pac) ){
                pac.normal = pac.ray.pt + pac.ray.dir*pac.t - pac.q;
                pac.hit    = true;
        }
}

SSTPacket sweep_unitsphere_against_tri(Ray3CR ray, Tri3CR tri)
{
        SSTPacket pac; pac.ray = ray; pac.tri = tri;

        deny(pierce(Sphere(ray.pt, 1), tri));
        sweep_unitsphere_against_tri_work(pac);
        assert(!pierce(Sphere(ray.b(), 1), tri) || pac.hit);

        pac.dist = pac.t * ray.dir.length();
        return pac;
}

bool test(Pt3& p, Tri3CR t, SphereCR s) {return (p = t.closest_pt_to(s.c)).sqr_dist_to(s.c) <= sqr(s.r);}

bool test(Tri3CR tri, SphereCR s)
{
        Vec3 n = tri.normal();
        deny(n.is_zero());

        real d            = (s.c - tri.a).dot(n);  // find nearest feature on tri to sphere
        bool Int          = d >= -s.r && d <= s.r; // if center of sphere is farther from plane of tri than radius of sphere, then there cannot be an intersection
        Pt3  pt           = s.c - n*d;             // project center of sphere onto plane of tri
        bool intersection = tri.contains3(pt);     // is it in all edges? If so we intersect because dist to plane is less than radius
        real radSq        = sqr(s.r);              // find nearest pt on each edge

        pt = ptOnLineSegmentNearestpt(Seg3(tri.a, tri.b), s.c); intersection |= s.c.sqr_dist_to(pt) <= radSq; // if dist to center of sphere to pt is less than rad of sphere then it must intersec Edge 0,1
        pt = ptOnLineSegmentNearestpt(Seg3(tri.b, tri.c), s.c); intersection |= s.c.sqr_dist_to(pt) <= radSq; // if dist to center of sphere to pt is less than rad of sphere then it must intersec Edge 1,2
        pt = ptOnLineSegmentNearestpt(Seg3(tri.c, tri.a), s.c); intersection |= s.c.sqr_dist_to(pt) <= radSq; // edge 2,0
        return intersection && Int; // if dist to center of sphere to pt is less than radius of sphere then it must intersect
}