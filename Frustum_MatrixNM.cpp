#include "stdafx.h"
#include "Frustum_MatrixNM.h"
#include "Frustum.h"
#include "MatrixNM.h"
#include "MatrixNM_Vec.h"

Frustum6 create_frustum6(Matrix44CR VP)
{
        Frustum6 frus(Plane(-VP._03 -VP._02, -VP._13 -VP._12, -VP._23 -VP._22, VP._33 +VP._32),
                      Plane(-VP._03 +VP._02, -VP._13 +VP._12, -VP._23 +VP._22, VP._33 -VP._32),
                      Plane(-VP._03 -VP._00, -VP._13 -VP._10, -VP._23 -VP._20, VP._33 +VP._30),
                      Plane(-VP._03 +VP._00, -VP._13 +VP._10, -VP._23 +VP._20, VP._33 -VP._30),
                      Plane(-VP._03 +VP._01, -VP._13 +VP._11, -VP._23 +VP._21, VP._33 -VP._31),
                      Plane(-VP._03 -VP._01, -VP._13 -VP._11, -VP._23 -VP._21, VP._33 +VP._31));
        for( auto& i : frus.p ) i.normalize();
        return frus;
}

FrustumQ create_frustumQ_from_perspective_matrix(Matrix44CR P)
{
        // corners of proj frustum in homogenous space
        const Pt4 homopts[] = {Pt4( 1,  0, 1, 1), // right (at far plane)
                               Pt4(-1,  0, 1, 1), // left
                               Pt4( 0,  1, 1, 1), // top
                               Pt4( 0, -1, 1, 1), // bottom
                               Pt4( 0,  0, 0, 1),  // near
                               Pt4( 0,  0, 1, 1)}; // far

        auto iP = P.get_inverse();

        Pt4 pts[6]; // compute frustum corners in world space
        DO_TIMES(i, 6) pts[i] = homopts[i] * iP;

        pts[0] /= pts[0].z; pts[1] /= pts[1].z; // compute slopes
        pts[2] /= pts[2].z; pts[3] /= pts[3].z;
        pts[4] /= pts[4].w; pts[5] /= pts[5].w; // compute near and far

        return FrustumQ(VEC3_ZERO, QUAT_IDENT, pts[0].x, pts[1].x, pts[2].y, pts[3].y, pts[4].z, pts[5].z);
}