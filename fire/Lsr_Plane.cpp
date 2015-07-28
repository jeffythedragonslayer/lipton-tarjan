#include "stdafx.h"
#include "Lsr_Plane.h"
#include "Lsr.h"
#include "Plane.h"
using namespace std;

bool segcast_plane(Seg3CR seg, PlaneCR p, real& t, Pt3& q)
{
        t = (p.d - p.n.dot(seg.a)) / p.n.dot(seg.dir()); // compute t val for directed line seg intersecting plane
        return t >= 0 && t <= 1         ?
               q = seg.eval_at(t), true :
               false;
}

bool raycast_plane(Ray3CR ray, PlaneCR p, real& t, Pt3& q) // WARNING: these tests do not check for parallel ray with plane
{
        t = (p.d - p.n.dot(ray.pt)) / p.n.dot(ray.dir); // compute t val for directed line seg intersecting plane
        if( t >= 0 ){
                q = ray.eval_at(t);
                return true;
        }
        return false;
}

bool linecast_plane(Ray3CR line, PlaneCR p, real& t, Pt3& q)
{
        t = (p.d - p.n.dot(line.pt)) / p.n.dot(line.dir); // compute t val for directed line seg intersecting plane
        q = line.eval_at(t);
        return true;
}