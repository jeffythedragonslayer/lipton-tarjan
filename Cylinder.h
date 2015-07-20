#pragma once
#include "Lsr.h"

struct Cylinder 
{
        Seg3 seg;
        real r;

        Cylinder         (Seg3CR, real r);

        real height_sqr  ()       const;
        real height      ()       const;
        real surface_area()       const;
        real volume      ()       const;
        Pt3  support_pt  (Vec3CR) const;
};

/*Least-squares fit of a finite cylinder to (x,y,z) data.  The cylinder has center C, unit-length axis direction U, radius r, and height h.  The end disks of the cylinder are at C+(h/2)*U and C-(h/2)*U.  A pt X is on the cylinder wall if (X-C)^T*(I-U*U^T)*(X-C) = r^2.  A pt X is on the end disk at C+(h/2)*U if dot(U,X-C) = h/2 and (X-C)^T*(I-U*U^T)*(X-C) <= r^2.  A pt X is on the end disk at C-(h/2)*U if dot(U,X-C) = -h/2 and (X-C)^T*(I-U*U^T)*(X-C) <= r^2.  The inputs are the quantity of points and the pt array.  The outputs
are the center C, unit-length axis direction U, radius R, and height H.  You can supply initial guesses for C and U.  In this case you need to set bInputsAreInitialGuess to 'true'.  Otherwise set it to 'false' and the function will select C and U by first fitting the data with a least-squares line.  The return function val is the error for the least-squares fit, e >= 0.  If all the points lie exactly on a cylinder, then e = 0.  You can examine the error e and iterate the calls yourself.  The outputs C, U, R, and H can be fed back into the function call as initial guesses.
real error0 = CylinderFit3<real>(num_pts,points,C,U,R,H,false);
for (i = 1; i <= imax; i++)
{
    real error1 = CylinderFit3<real>(num_pts,points,C,U,R,H,true);
    if (eError1 not changed much from error0)
        break;
}*/

struct CylinderFit
{
        CylinderFit(uint num_pts, Vec3CP points, Vec3& center, Vec3& axis, real& radius, real& height, bool inputsAreInitialGuess);

        operator real (); // Return the error val

private:
        static real UpdateInvRSqr(uint num_pts, Vec3CP points, Vec3CR center, Vec3CR axis, real& invRadiusSqr); 
        static real UpdateDir    (uint num_pts, Vec3CP points, Vec3CR center, Vec3&  axis, real& invRadiusSqr); 
        static real UpdateCenter (uint num_pts, Vec3CP points, Vec3&  center, Vec3CR axis, real  invRadiusSqr);

        real err;
};
