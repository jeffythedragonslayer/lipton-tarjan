#include "stdafx.h"
#include "MatrixNM_Vec.h"
#include "Vec.h"
#include "MatrixNM.h"
using namespace std;

Matrix22 create_matrix22_from_rows(Vec2CR _0, Vec2CR _1) {return Matrix22(_0.x, _0.y, _1.x, _1.y);} 
Matrix22 create_matrix22_from_cols(Vec2CR _0, Vec2CR _1) {return Matrix22(_0.x, _1.x, _0.y, _1.y);}

Matrix23 create_matrix33_from_row_vec2s(Vec2CR _1, Vec2CR _2, Vec2CR _3) {return Matrix23(_1.x, _1.y, _2.x, _2.y, _3.x, _3.y);}
Matrix23 create_matrix33_from_col_vec3s(Vec3CR _1, Vec3CR _2)            {return Matrix23(_1.x, _2.x, _1.y, _2.y, _1.z, _2.z);}

Vec2 get_row_vec2(Matrix22CR m, uint i) {return Vec2(m.data[i][0], m.data[i][1]);}
Vec2 get_col_vec2(Matrix22CR m, uint i) {return Vec2(m.data[0][i], m.data[1][i]);} 

Vec2 get_row_vec2(Matrix23CR m, uint i) {return Vec2(m.data[i][0], m.data[i][1]);} 
Vec3 get_col_vec3(Matrix23CR m, uint i) {return Vec3(m.data[0][i], m.data[1][i], m.data[2][i]);}

Matrix33 direct_product(Vec3CR a, Vec3CR b)
{ // right handed?
        return Matrix33(a.x*b.x, a.x*b.y, a.x*b.z,
                        a.y*b.x, a.y*b.y, a.y*b.z,
                        a.z*b.x, a.z*b.y, a.z*b.z);
}

Vec3  operator* (Vec3CR v, Matrix33CR m) {return Vec3(v.x*m._00 + v.y*m._10 + v.z*m._20,
                                                      v.x*m._01 + v.y*m._11 + v.z*m._21,
                                                      v.x*m._02 + v.y*m._12 + v.z*m._22);}

Vec3& operator*=(Vec3& v, Matrix33CR m) {return v = v * m;}

Matrix33 rotation_matrix(Vec3CR axis, real angle)
{
        real s  = sin(angle);
        real c  = cos(angle);
        real ic = 1 - c;

        return Matrix33(ic*axis.x*axis.x + c,        ic*axis.x*axis.y + s*axis.z, ic*axis.x*axis.z - s*axis.y,
                        ic*axis.x*axis.y - s*axis.z, ic*axis.y*axis.y + c,        ic*axis.y*axis.z + s*axis.x,
                        ic*axis.x*axis.z + s*axis.y, ic*axis.y*axis.z - s*axis.x, ic*axis.z*axis.z + c);
}

Vec3 get_scaling(Matrix34CR m) {return get_scaling(Matrix33(m));}

Vec3 get_scaling(Matrix33CR m)
{
        return Vec3(sqrt(sqr(m._00) + sqr(m._10) + sqr(m._20)), // if you know that scaling was uniform, you could save some cycles by calculating det of 3x3 minor instead
                    sqrt(sqr(m._01) + sqr(m._11) + sqr(m._21)),
                    sqrt(sqr(m._02) + sqr(m._12) + sqr(m._22)));
}

Matrix33 scaling_matrix(Vec3CR scaling_factors) {return Matrix33::scaling(scaling_factors.x, scaling_factors.y, scaling_factors.z);}

Matrix33 create_matrix33_from_rows(Vec3CR _0, Vec3CR _1, Vec3CR _2) {return Matrix33(_0.x, _0.y, _0.z, _1.x, _1.y, _1.z, _2.x, _2.y, _2.z);} 
Matrix33 create_matrix33_from_cols(Vec3CR _0, Vec3CR _1, Vec3CR _2) {return Matrix33(_0.x, _1.x, _2.x, _0.y, _1.y, _2.y, _0.z, _1.z, _2.z);}

Matrix33 create_matrix33_skew_symmetric(Vec3CR v)
{
        return Matrix33(0,    v.z, -v.y,
                       -v.z,  0,    v.x,
                        v.y, -v.x,  0);
}

Vec3 get_row_vec3(Matrix33CR m, uint r) {return Vec3(m.data[r][0], m.data[r][1], m.data[r][2]);}
Vec3 get_col_vec3(Matrix33CR m, uint c) {return Vec3(m.data[0][c], m.data[1][c], m.data[2][c]);}
 
Matrix34 change_of_frame_matrix        (Vec3CR u, Vec3CR v, Vec3CR w, Pt3CR q) {return Matrix34(u.x, u.y, u.z, v.x, v.y, v.z, w.x, w.y, w.z, q.x,       q.y,       q.z);}
Matrix34 reverse_change_of_frame_matrix(Vec3CR u, Vec3CR v, Vec3CR w, Pt3CR q) {return Matrix34(u.x, v.x, w.x, u.y, v.y, w.y, u.z, v.z, w.z, -u.dot(q), -v.dot(q), -w.dot(q));}

Matrix34 lookatLH(Vec3CR eye, Vec3CR at, Vec3CR up) {Vec3 z = (at - eye).normal(); Vec3 x = up.cross(z).normal(); Vec3 y = z.cross(x); return Matrix34(x.x, y.x, z.x, x.y, y.y, z.y, x.z, y.z, z.z, -x.dot(eye), -y.dot(eye), -z.dot(eye));} 
Matrix34 lookatRH(Vec3CR eye, Vec3CR at, Vec3CR up) {Vec3 z = (eye - at).normal(); Vec3 x = up.cross(z).normal(); Vec3 y = z.cross(x); return Matrix34(x.x, y.x, z.x, x.y, y.y, z.y, x.z, y.z, z.z,  x.dot(eye),  y.dot(eye),  z.dot(eye));}

Pt3      get_translation   (Matrix34CR m) {return Vec3(m._30, m._31, m._32);}
Matrix34 translation_matrix(Vec3CR     v) {return Matrix34::translation(v.x, v.y, v.z);}

Vec3 operator* (Vec3CR v, Matrix34CR m)
{
        return Vec3(v.x * m._00 + v.y * m._10 + v.z * m._20 + m._30,
                    v.x * m._01 + v.y * m._11 + v.z * m._21 + m._31,
                    v.x * m._02 + v.y * m._12 + v.z * m._22 + m._32);
}

Vec3& operator*=(Vec3& v, Matrix34CR m) {return v = v * m;}

Vec3 transform_inverse(Vec3CR v, Matrix34CR m)
{
        Vec3 tmp(v.x - m._30, v.y - m._31, v.z - m._32);
        return Vec3(tmp.x * m._00 + tmp.y * m._01 + tmp.z * m._02,
                    tmp.x * m._10 + tmp.y * m._11 + tmp.z * m._12,
                    tmp.x * m._20 + tmp.y * m._21 + tmp.z * m._22);
}

Vec3 transform_inverse_direction(Vec3CR v, Matrix34CR m)
{
        return Vec3(v.x * m._00 + v.y * m._01 + v.z * m._02,
                    v.x * m._10 + v.y * m._11 + v.z * m._12,
                    v.x * m._20 + v.y * m._21 + v.z * m._22);
}

Matrix34 create_matrix34_from_rows(Vec3CR _0, Vec3CR _1, Vec3CR _2, Vec3CR _3) {return Matrix34(_0.x, _0.y, _0.z, _1.x, _1.y, _1.z, _2.x, _2.y, _2.z, _3.x, _3.y, _3.z);} 
Matrix34 create_matrix34_from_cols(Vec4CR _0, Vec4CR _1, Vec4CR _2)            {return Matrix34(_0.x, _1.x, _2.x, _0.y, _1.y, _2.y, _0.z, _1.z, _2.z, _0.w, _1.w, _2.w);}

Vec3 get_row_vec3(Matrix34CR m, uint r) {return Vec3(m.data[r][0], m.data[r][1], m.data[r][2]);} 
Vec4 get_col_vec4(Matrix34CR m, uint c) {return Vec4(m.data[0][c], m.data[1][c], m.data[2][c], m.data[3][c]);}

Vec3 operator* (Vec3CR v, Matrix44CR m)
{
        return Vec3(v.x * m._00 + v.y * m._10 + v.z * m._20 + m._30,
                    v.x * m._01 + v.y * m._11 + v.z * m._21 + m._31,
                    v.x * m._02 + v.y * m._12 + v.z * m._22 + m._32);
}

Vec4  operator* (Vec4CR v, Matrix44CR m) {Vec4 ret  = VEC4_ORIGIN; DO_TIMES(c, 4) DO_TIMES(i, 4)  ret[c] += v[i] * m(i, c); return ret;}
Vec4& operator*=(Vec4&  v, Matrix44CR m) {Vec4 newv = VEC4_ZERO;   DO_TIMES(c, 4) DO_TIMES(i, 4) newv[c] += v[i] * m(i, c); return v = newv;}

Matrix44 create_matrix44_from_rows(Vec4CR _0, Vec4CR _1, Vec4CR _2, Vec4CR _3) {return Matrix44(_0.x, _0.y, _0.z, _0.w, _1.x, _1.y, _1.z, _1.w, _2.x, _2.y, _2.z, _2.w, _3.x, _3.y, _3.z, _3.w);} 
Matrix44 create_matrix44_from_cols(Vec4CR _0, Vec4CR _1, Vec4CR _2, Vec4CR _3) {return Matrix44(_0.x, _1.x, _2.x, _3.x, _0.y, _1.y, _2.y, _3.y, _0.z, _1.z, _2.z, _3.z, _0.w, _1.w, _2.w, _3.w);}

Vec4 get_row_vec3(Matrix44CR m, uint r) {return Vec4(m.data[r][0], m.data[r][1], m.data[r][2], m.data[r][3]);}
Vec4 get_col_vec3(Matrix44CR m, uint c) {return Vec4(m.data[0][c], m.data[1][c], m.data[2][c], m.data[3][c]);}