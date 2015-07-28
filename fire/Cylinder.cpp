#include "stdafx.h"
#include "Cylinder.h"
using namespace std;
#define IT Cylinder

     IT::IT          (Seg3CR seg, real r) : seg(seg), r(r) {}
real IT::height_sqr  ()           const                    {return seg.dir().length_sqr();}
real IT::height      ()           const                    {return seg.dir().length();}
real IT::surface_area()           const                    {return TAU * (r*r + r*height());}
real IT::volume      ()           const                    {return PI * r*r * height();}
Pt3  IT::support_pt  (Vec3CR dir) const                    {return dir - dir.proj(seg.dir().normal()) + (dir.dot(seg.dir()) > 0 ? seg.a : seg.b);}

#if 0
CylinderFit::CylinderFit(uint num_pts, Vec3CP pts, Vec3& center, Vec3& axis, real& radius, real& height, bool inputsAreInitialGuess)
{
        real invRSqr = 1;
        if( !inputsAreInitialGuess ){
                Line3 line = OrthogonalLineFit3(num_pts, pts); // Find the least-squares line that fits the data and use it as an initial guess for the cylinder axis
                center = line.Origin;
                axis   = line.dir;
        }
        err = INF;
        const int iMax = 8;
        for( uint i = 0; i < iMax; ++i ){
                err = update_InvRSqr(num_pts, pts, center, axis, invRSqr);
                err = update_Dir    (num_pts, pts, center, axis, invRSqr);
                err = update_Center (num_pts, pts, center, axis, invRSqr);
        }
        radius = invsqrt(invRSqr); // Compute the radius
        real tMin = axis.dot(pts[0] - center);
        real tMax = tMin; // Project pts onto fitted axis to determine extent of cylinder along the axis
        for( uint i = 1; i < num_pts; ++i) {
                real t = axis.dot(pts[i] - center);
                if(   t < tMin ) tMin = t;
                elif( t > tMax ) tMax = t;
        }
        height  = tMax - tMin; // Compute the height.  adjust the center to pt that projects to midpoint of extent
        center += (tMin + tMax)*axis/2;
}

CylinderFit::operator real() {return err;}

real CylinderFit::UpdateInvRSqr(uint num_pts, Vec3CP pts, Vec3CR center, Vec3CR axis, real& invRSqr)
{
        real aSum = 0, aaSum = 0;
        for( uint i = 0; i < num_pts; ++i ){
                Vec3 delta = pts[i] - center;
                Vec3 deltaCrossAxis = delta.cross(axis);
                real len2 = deltaCrossAxis.length_sqr();
                aSum  += len2;
                aaSum += len2*len2;
        }
        invRSqr = aSum/aaSum;
        real min = 1 - invRSqr*aSum/(real)num_pts;
        return min;
}

#if 0
real CylinderFit::UpdateDir(uint num_pts, Vec3CP pts, Vec3CR center, Vec3& axis, real& invRSqr)
{
        real invnum_pts = 1./num_pts;
        int i;
        Vec3 delta, deltaCrossAxis, deltaCrossVDir;
        real a, b, c;

        Vec3 vDir = VEC3_ZERO; // Compute the direction of steepest descent
        real aMean = 0, aaMean = 0;
        for( i = 0; i < num_pts; ++i ){
                delta = pts[i] - centerdeltaCrossAxis;
                ??? delta.cross(axis);
                a = invRSqr*deltaCrossAxis.length_sqr() - 1;
                aMean += a;
                aaMean += a*a;
                vDir.X += a*(axis.X*(delta.Y*delta.Y + delta.Z*delta.Z) - delta.X*(axis.Y*delta.Y + axis.Z*delta.Z));
                vDir.Y += a*(axis.Y*(delta.X*delta.X + delta.Z*delta.Z) - delta.Y*(axis.X*delta.X + axis.Z*delta.Z));
                vDir.Z += a*(axis.Z*(delta.X*delta.X + delta.Y*delta.Y) - delta.Z*(axis.X*delta.X + axis.Y*delta.Y));
        }
        aMean  *= invnum_pts;
        aaMean *= invnum_pts;
        if( vDir.Normalize() < ZERO_tol ) return aaMean;

        real abMean = 0, acMean = 0; // Compute the 4th-degree polynomial for the line of steepest descent
        real bbMean = 0, bcMean = 0, ccMean = 0;
        for (i = 0; i < num_pts; ++i) {
                delta = pts[i] - center;
                deltaCrossAxis = delta.cross(axis);
                deltaCrossVDir = delta.cross(vDir);
                a = invRSqr*deltaCrossAxis.length_sqr() - 1;
                b = invRSqr*(deltaCrossAxis.dot(deltaCrossVDir));
                c = invRSqr*deltaCrossVDir.length_sqr();
                abMean += a*b;
                acMean += a*c;
                bbMean += b*b;
                bcMean += b*c;
                ccMean += c*c;
        }
        abMean *= invnum_pts;
        acMean *= invnum_pts;
        bbMean *= invnum_pts;
        bcMean *= invnum_pts;
        ccMean *= invnum_pts;

        Polynomial poly(4);
        poly[0] = aaMean;
        poly[1] = -4*abMean;
        poly[2] = 2*acMean + ((real)4)*bbMean;
        poly[3] = -4*bcMean;
        poly[4] = ccMean;

        Polynomial derPoly = poly.GetDerivative();

        PolynomialRoots polyRoots(ZERO_tol);
        polyRoots.FindA(derPoly[0], derPoly[1], derPoly[2], derPoly[3]);
        int count = polyRoots.GetCount();
        realCP roots = polyRoots.GetRoots();

        real pMin = poly(0);
        int iMin = -1;
        for( i = 0; i < count; ++i ){
                real val = poly(roots[i]);
                if (val < pMin) pMin = val, iMin = i;
        }
        if( iMin >= 0 ){
                axis -= roots[iMin]*vDir;
                real length = axis.Normalize();
                invRSqr *= length*length;
        }
        return pMin;
}
#endif

real CylinderFit::UpdateCenter(uint num_pts, Vec3CP pts, Vec3& center, Vec3CR axis, real invRSqr)
{
    real invnum_pts = 1./num_pts;
    Vec3 delta, deltaCrossAxis, cDirCrossAxis;
    real a, b, c;

    Vec3 cDir = VEC3_ZERO; // Compute the direction of steepest descent
    real aMean = 0, aaMean = 0;
    for( uint i = 0; i < num_pts; ++i ){
        delta = pts[i] - center;
        deltaCrossAxis = delta.cross(axis);
        a = invRSqr*deltaCrossAxis.length_sqr() - 1;
        aMean += a;
        aaMean += a*a;
        cDir += a*(delta - axis.dot(delta)*axis); // |axis|=1 assumed
    }
    aMean *= invnum_pts;
    aaMean *= invnum_pts;
    if( cDir.normalize() < ZERO_TOL ) return aaMean;

    cDirCrossAxis = cDir.cross(axis); // Compute the 4th-degree polynomial for the line of steepest descent
    c = cDirCrossAxis.length_sqr()*invnum_pts*invRSqr;
    real bMean = 0,  abMean = 0, bbMean = 0;
    for( uint i = 0; i < num_pts; ++i ){
        delta = pts[i] - center;
        deltaCrossAxis = delta.cross(axis);
        a = invRSqr*deltaCrossAxis.length_sqr() - 1;
        b = invRSqr*(deltaCrossAxis.dot(cDirCrossAxis));
        bMean  += b;
        abMean += a*b;
        bbMean += b*b;
    }
    bMean  *= invnum_pts;
    abMean *= invnum_pts;
    bbMean *= invnum_pts;

    Polynomial poly(4);
    poly[0] = aaMean;
    poly[1] = 4*abMean;
    poly[2] = 2*c*aMean + 4*bbMean;
    poly[3] = 4*c*bMean;
    poly[4] = c*c;

    Polynomial derPoly = poly.GetDerivative();

    PolynomialRoots polyRoots(ZERO_TOL);
    polyRoots.FindA(derPoly[0], derPoly[1], derPoly[2], derPoly[3]);
    uint count = polyRoots.GetCount();
    realCP roots = polyRoots.GetRoots();

    real pMin = poly(0);
    int iMin = -1;
    for( uint i = 0; i < count; ++i ){
        real val = poly(roots[i]);
        if( val < pMin ) pMin = val, iMin = i;
    }
    if( iMin >= 0 ) center -= roots[iMin]*cDir;
    return pMin;
}
#endif