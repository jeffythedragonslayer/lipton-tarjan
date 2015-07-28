#include "stdafx.h"
#include "Lsr_Quad.h"
#include "BaryPt.h"
#include "Quad.h"
#include "Lsr.h"
using namespace std;

bool linecast_quad(Ray3CR line, Quad3CR q, Pt3& r) // given line pq and ccw quadrilateral abcd, return whether line pierces tri.  if so, also return pt r of intersection
{
        Vec3   pa = q.a - line.pt; 
        Vec3   pb = q.b - line.pt;
        Vec3   pc = q.c - line.pt;
        Vec3   m  = pc.cross(line.dir);  // determine which tri to test against by testing against diagonal first
        BaryPt bary;
        if( (bary.v = pa.dot(m)) >= 0 ){ // box(pq, pa, pc); test intersection against tri abc
                if( (bary.u = -pb.dot(m))           < 0 ) return false; // box(pq, pc, pb);
                if( (bary.w = line.dir.box(pb, pa)) < 0 ) return false;
                r = bary.normalize().get_pt(q.a, q.b, q.c);
        } else { // test intersection against tri dac
                Vec3 pd = q.d - line.pt;
                if( (bary.u = pd.dot(m))            < 0 ) return false; // box(pq, pd, pc);
                if( (bary.w = line.dir.box(pa, pd)) < 0 ) return false;
                bary.v = -bary.v;
                r      = bary.normalize().get_pt(q.a, q.d, q.c);
        }
        return true;
}