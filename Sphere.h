#pragma once
#include "Vec.h"
#include <vector>

struct Circle
{
        Pt2  c;
        real r;

             Circle        ();
             Circle        (Pt2CR, real);
             Circle        (Pt2CR);
             Circle        (Pt2CR, Pt2CR);
             Circle        (Pt2CR, Pt2CR, Pt2CR);
        real area          ()      const;
        real circumference ()      const;
        bool contains      (Pt2CR) const;
};

real in_circle2d (Pt2CR a, Pt2CR b, Pt2CR c, Pt2CR d);

// Least-squares fit of a circle to a set of points.  A successful fit is indicated by the return val of 'true'.  If the return val is 'false', the number of iterations was exceeded.  Try increasing the maximum number of iterations
// If initialCenterIsmean is set to 'true', the initial guess for the circle center is the mean of the data points.  If the data points are clustered along a small arc, CircleFit2 is very slow to converge.  If initialCenterIsmean is set to 'false', the initial guess for the circle center is computed using a least-squares estimate of the coefficients for a quadratic equation that represents a circle.  This approach tends to converge rapidly
bool CircleFit2(Circle&, uint num_pts, Vec2CP points, uint maxIters, bool initialCenterIsmean);

struct Sphere
{
        Pt3  c;
        real r;

                      Sphere            ();
                      Sphere            (Pt3CR c, real r);
                      Sphere            (Pt3CR);
                      Sphere            (Pt3CR, Pt3CR);
                      Sphere            (Pt3CR, Pt3CR, Pt3CR);
                      Sphere            (Pt3CR, Pt3CR, Pt3CR, Pt3CR);
        static Sphere bad_create        (std::vector<Pt3> const&);
        static Sphere ritter_create     (std::vector<Pt3> const&);
        static Sphere welzel_create     (std::vector<Pt3> const&);
        static Sphere merge             (SphereCR, SphereCR);

        Pt3           support_pt        (Vec3CR)                                      const;
        Pt3           closest_pt_to     (Pt3CR)                                       const;
        real          sqr_dist_to_center(Pt3CR)                                       const;
        bool          contains          (Pt3CR)                                       const;
        bool          contains          (SphereCR)                                    const;
        bool          test              (SphereCR)                                    const;
        bool          sweep             (real& t, Vec3CR avel, SphereCR, Vec3CR bvel) const;

        void          merge             (Pt3CR);

        real          volume            ()                                            const;
        real          surface_area      ()                                            const;
};

real in_sphere       (Pt3CR a, Pt3CR b, Pt3CR c, Pt3CR d, Pt3CR e);
void build_geo_sphere(std::vector<Vec3>& verts, IndexList16&, uint subdivs, real rad);

// Least-squares fit of a sphere to a set of points.  A successful fit is indicated by the return val of 'true'.  If the return val is 'false', the number of iterations was exceeded.  Try increasing the maximum number of iterations
// If initialCenterIsmean is set to 'true', the initial guess for the sphere center is the mean of the data points.  If data points are clustered along a solid angle, SphereFit3 is very slow to converge.  If initialCenterIsmean is set to 'false', the initial guess for the sphere center is computed using a least-squares estimate of the coefficients for a quadratic equation that represents a sphere.  This approach tends to converge rapidly
bool sphere_fit(uint num_pts, Vec3CP pts, uint max_iters, Sphere&, bool initialCenterIsmean);
