#include "stdafx.h"
#include "MatrixNM_Quat.h"
#include "MatrixNM.h"
#include "Quat.h"
using namespace std;

Matrix33 create_matrix33(QuatCR q)
{
        Vec3 v2  = q.v   * 2;
        Vec3 vv2 = q.v.component_product(v2);
        real xy  = q.v.x * v2.y;
        real xz  = q.v.x * v2.z;
        real yz  = q.v.y * v2.z;
        Vec3 wv2 = q.w   * v2;
        return Matrix33(1 - vv2.y - vv2.z, xy - wv2.z,        xz + wv2.y,
                        xy + wv2.z,        1 - vv2.x - vv2.z, yz - wv2.x,
                        xz - wv2.y,        yz + wv2.x,        1 - vv2.x - vv2.y);
}

Quat create_quat(Matrix33CR m)
{
        real trace = m.trace();
        if( trace > 0 ){
                real s = sqrt(trace + 1);
                Vec3   v(m._12 - m._21,
                         m._20 - m._02,
                         m._01 - m._10);
                return Quat(v / s, s) / 2;
        }
        uint i = 0;
        if( m._11 > m._00   ) i = 1;
        if( m._22 > m(i, i) ) i = 2;
        uint   j = (i+1)%3;
        uint   k = (j+1)%3;
        real s = sqrt(m(i, i) - m(j, j) - m(k, k) + 1);
        real t = s ? .5 / s : 0;
        return Quat(s / 2,
                    t * (m(i,j) - m(j, i)),
                    t * (m(i,k) - m(k, i)),
                    t * (m(j,k) - m(k, j)));
}

Matrix34 create_matrix34(QuatCR q, Pt3CR pos)
{
        Vec3   v2  = q.v * 2;
        Vec3   vv2 = q.v.component_product(v2);
        real xy  = q.v.x * v2.y;
        real xz  = q.v.x * v2.z;
        real yz  = q.v.y * v2.z;
        Vec3   wv2 = q.w * v2;
        return Matrix34(1 - vv2.y - vv2.z, xy - wv2.z,        xz + wv2.y,
                        xy + wv2.z,        1 - vv2.x - vv2.z, yz - wv2.x,
                        xz - wv2.y,        yz + wv2.x,        1 - vv2.x - vv2.y,
                        pos.x,             pos.y,             pos.z);
}