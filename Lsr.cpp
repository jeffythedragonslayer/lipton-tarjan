#include "stdafx.h"
#include "Lsr.h"
using namespace std;

Vec2 Seg2::dir       () const                        {return b - a;}
real Seg2::length_sqr() const                        {return dir().length_sqr();}
real Seg2::length    () const                        {return dir().length();}
Pt2  Seg2::midpoint  () const                        {return (a + b)/2;}

bool Seg2::test(real& t, Pt2& p, Seg2CR s2) const // compute and return intersection t val along s1 and intersection position p
{
        real a1 = Tri2(a, b, s2.b).signed_area(); // sgn of areas correspond to which side of ab pts c and d are
        real a2 = Tri2(a, b, s2.a).signed_area(); // compute winding of abd (+ or -) to intersect, must have sgn opposite of a1

        if( a1 && a2 && a1 * a2 < 0 ){  // if c and d are on different sides of ab, areas have different signs

                // compute signs for a and b with respect to segment cd
                real a3 = Tri2(s2.a, s2.b, a).signed_area(); // compute winding of cda (+ or -)

                // Since area is constant a1-a2 = a3-a4, or a4=a3+a2-a1
//              real a4 = Signed2DTriArea(c, d, b); // Musc have opposite sgn of a3
                real a4 = a3 + a2 - a1;

                if( a3 * a4 < 0 ){  // pts a and b on different sides of cd if areas have different signs
                        // Segments intersect. Find intersection pt along L(t)=a+t*(b-a). Given height h1 of a over cd and height h2 of b over cd, t = h1 / (h1 - h2) = (b*h1/2) / (b*h1/2 - b*h2/2) = a3 / (a3 - a4),
                        // where b (base of tris cda and cdb, i.e., length of cd) cancels out.
                        t = a3 / (a3 - a4);
                        p = a + t * dir();
                        return true;
                }
        }
        return false; // segments not intersecting (or collinear)
}

Vec3 Seg3::dir       ()       const                  {return b - a;}
real Seg3::length_sqr()       const                  {return dir().length_sqr();}
real Seg3::length    ()       const                  {return dir().length();}
Pt3  Seg3::eval_at   (real t) const                  {return a*(1-t) + b*t;}
Pt3  Seg3::midpoint  ()       const                  {return (a + b)/2;}

Pt3 Seg3::closest_pt_to(Pt3CR c, real& t) const
{
        t = (c - a).dot(dir()); // project c onto ab, but deferring divide by ab.length_sqr()

        if( t <= 0 ){
                t = 0; // c projects outside [a,b] interval, on a side; clamp to a
                return a;
        }
        real denom = length_sqr(); // always nonnegative since denom = ||ab||^2
        if( t >= denom ){
                t = 1; // c projects outside [a,b] interval, on b side; clamp to b
                return b;
        }
        t /= denom; // c projects in [a,b] interval; must do deferred divide now
        return eval_at(t);
}

real Seg3::sqr_dist_to(Pt3CR c) const
{
        Vec3 ac = c - a;
        Vec3 bc = c - b;
        real e  = ac.dot(dir());
        if( e <= 0 ) return ac.length_sqr(); // handle cases where c projects outside ab
    
        real f = length_sqr();
        return e >= f ? bc.length_sqr() : ac.length_sqr() - e*e / f; // handle case where c projects onto ab
}

real Seg3::closest_pt_to_seg(Seg3CR s2, real &s, real &t, Pt3 &c1, Pt3 &c2) const // computes closest pts C1 and C2 of s1 and s2 rting s and t. Func result is sqr dist between between S1(s) and S2(t)
{
        Vec3 r = self.a - s2.a;
        real a = length_sqr();
        real e = s2.length_sqr();
        real f = s2.dir().dot(r);

        if( a <= EP && e <= EP ){ // both segments degenerate into pts
                s  = t = 0;
                c1 = self.a; 
                c2 = s2.a;
                return c1.sqr_dist_to(c2);
        }
        if( a <= EP ) s = 0, t = saturate(f / e); // first segment degenerates into a pt s = 0 => t = (b*s + f) / e = f / e
        else {
                real c = dir().dot(r);
                if( e <= EP ){ // second segment degenerates into a pt
                        t = 0;
                        s = saturate(-c / a); // t = 0 => s = (b*t - c) / a = -c / a
                } else { // general nondegenerate case starts here
                        real b     = dir().dot(s2.dir());             
                        real denom = a*e - b*b; // always nonnegative

                        s = denom ? saturate((b*f - c*e) / denom) : 0; // if segments not parallel, compute closest pt on L1 to L2, and clamp to segment S1. Else pick arbitrary s (here 0)

                        real tnom = b*s + f;
                        if(   tnom < 0 ) t = 0, s = saturate(-c / a);
                        elif( tnom > e ) t = 1, s = saturate((b - c) / a);
                        else t = tnom / e;
                }
        }
        c1 =    eval_at(s);
        c2 = s2.eval_at(t);
        return c1.sqr_dist_to(c2);
}

Pt3 ptOnLineSegmentNearestpt(Seg3CR s, Pt3CR P)
{
        real proj = P.dot(s.dir()) - s.a.dot(s.dir());
        real s2   = s.length_sqr();
        real t    = proj / s2;
        Pt3  pt   = s.a + t * s.dir();
             pt   = proj < 0 ? s.a : pt;
        return proj > s2 ? s.b : pt;
}

     Ray2::Ray2     (Pt2CR pt, Vec2CR dir) : pt(pt), dir(dir) {}
Pt2  Ray2::b        ()       const                            {return pt + dir;}
Pt2  Ray2::eval_at  (real t) const                            {return pt + dir * t;}
Ray2 Ray2::inverse  ()       const                            {return Ray2(b(), -dir);}

     Ray3::Ray3     (Pt3CR pt, Vec3CR dir) : pt(pt), dir(dir) {}
bool Ray3::collinear(Pt3CR p) const                           {return dir.cross(p-pt).is_zero();}
Pt3  Ray3::b        ()        const                           {return pt + dir;}
Pt3  Ray3::eval_at  (real t)  const                           {return pt + dir * t;}
Ray3 Ray3::inverse  ()       const                            {return Ray3(b(), -dir);}

real Ray3::LINE_sqr_dist_to(Pt3CR c) const
{
        Vec3 ac = c - pt;
        real e  = ac.dot(dir);
        return ac.length_sqr() - e*e / dir.length_sqr();
}

Pt3 Ray3::LINE_closest_pt_to(real& t, Pt3CR c) const
{
        t = (c - pt).dot(dir) / dir.length_sqr();
        return eval_at(t);
}

real Ray3::LINE_closest_pt_to_line(Ray3CR s2, real& s, real& t, Pt3& c1, Pt3& c2) const
{
        Vec3 r     = self.pt - s2.pt;
        real a     = dir.length_sqr();
        real b     = dir.dot(s2.dir);
        real c     = dir.dot(r);
        real e     = s2.dir.length_sqr();
        real f     = s2.dir.dot(r);
        real denom = a*e - b*b;
        if( denom ) s = (b*f - c*e) / denom, t = (a*f - b*c) / denom; // lines are skew or intersect
        else        s = 0,                   t = b*s + f / e;         // lines are parallel, choose arbitrary s I hope this equation is right
        c1 = eval_at(s);
        c2 = s2.eval_at(t);
        return c1.sqr_dist_to(c2);
}