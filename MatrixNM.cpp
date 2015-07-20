#include "stdafx.h"
#include "MatrixNM.h"
#include "RuntimeErrors.h"
using namespace std;
#undef IT
#define IT Matrix22

     IT::IT              (realCP s)                                                                        {memcpy(data, s, sizeof(IT));}
     IT::IT              (real _00, real _01, real _10, real _11) : _00(_00), _01(_01), _10(_10), _11(_11) {} 
IT   IT::scaling         (real x, real y)                                                                  {return IT(x, 0, 0, y);} 
IT   IT::scaling         (real scale)                                                                      {return IT::scaling(scale, scale);}
void IT::transpose       ()                                                                                {swap(_01, _10);} 
IT   IT::get_transpose   () const                                                                          {return IT(_00, _10, _01, _11);} 
     IT::operator real*  ()                                                                                {return data[0];}
     IT::operator realCP () const                                                                          {return data[0];} 
real IT::trace           () const                                                                          {return _00 + _11;}
real IT::det             () const                                                                          {return _00 * _11 - _01 * _10;}

void IT::invert()
{
        real d = det(); if( !d ) throw Zerodet("MatrixNM::invert");
        d = 1 / d;
        swap(_00, _11);
        _00 *=  d; _01 *= -d;
        _10 *= -d; _11 *=  d;
}

IT IT::get_inverse() const
{
        real d = det();
        if( !d ) throw Zerodet("MatrixNM::get_inverse");
        d = 1 / d;
        return IT(_11 *  d, _01 * -d,
                  _10 * -d, _00 *  d);
}

IT& IT::operator+=(IT const& m) {_00 += m._00; _01 += m._01; _10 += m._10; _11 += m._11; return self;}
IT& IT::operator-=(IT const& m) {_00 -= m._00; _01 -= m._01; _10 -= m._10; _11 -= m._11; return self;}
IT& IT::operator*=(real s)      {_00 *= s;     _01 *= s;     _10 *= s;     _11 *= s;     return self;}
IT& IT::operator/=(real s)      {_00 /= s;     _01 /= s;     _10 /= s;     _11 /= s;     return self;}
IT  IT::operator* (IT const& m) const
{
        auto p = MATRIX22_ZERO;
        DO_TIMES(r, 2) DO_TIMES(c, 2) DO_TIMES(i, 3) p(r, c) += data[r][i] * m(i, c);
        return p;
}

IT&   IT::operator*=(IT const& m)         {return self = self * m;}
real& IT::operator()(uint r, uint c)       {assert(r  <2 && c  <2); return data[r]  [c];}
real  IT::operator()(uint r, uint c) const {assert(r  <2 && c  <2); return data[r]  [c];}
real& IT::operator()(Pt2u p)               {assert(p.x<2 && p.y<2); return data[p.x][p.y];}
real  IT::operator()(Pt2u p) const         {assert(p.x<2 && p.y<2); return data[p.x][p.y];}

#undef IT
#define IT Matrix23

IT::IT(real _00, real _01, real _10, real _11, real _20, real _21) : _00(_00),   _01(_01),   _10(_10),   _11(_11),   _20(_20),   _21(_21)   {} 
IT::IT(Matrix33CR m)                                               : _00(m._00), _01(m._01), _10(m._10), _11(m._11), _20(m._20), _21(m._21) {}

IT IT::translation(real x, real y) {auto m = MATRIX23_IDENT; m._20 = x; m._21 = y; return m;}

#undef IT
#define IT Matrix33

IT::IT(realCP s) {memcpy(data, s, sizeof(IT));} 
IT::IT(Matrix23CR m)                                                                             : _00(m._00), _01(m._01), _02(0),     _10(m._10), _11(m._11), _12(0),     _20(0),     _21(0),     _22(1)     {}
IT::IT(Matrix34CR m)                                                                             : _00(m._00), _01(m._01), _02(m._02), _10(m._10), _11(m._11), _12(m._12), _20(m._20), _21(m._21), _22(m._22) {}
IT::IT(Matrix44CR m)                                                                             : _00(m._00), _01(m._01), _02(m._02), _10(m._10), _11(m._11), _12(m._12), _20(m._20), _21(m._21), _22(m._22) {}
IT::IT(real _00, real _01, real _02, real _10, real _11, real _12, real _20, real _21, real _22) : _00(_00),   _01(_01),   _02(_02),   _10(_10),   _11(_11),   _12(_12),   _20(_20),   _21(_21),   _22(_22)   {}

IT  IT::xrotation (real angle)             {auto m = MATRIX33_IDENT; m._11 = m._22 = cos(angle); m._21 = -(m._12 = sin(angle)); return m;}
IT  IT::yrotation (real angle)             {auto m = MATRIX33_IDENT; m._00 = m._22 = cos(angle); m._02 = -(m._20 = sin(angle)); return m;}
IT  IT::zrotation (real angle)             {auto m = MATRIX33_IDENT; m._00 = m._11 = cos(angle); m._10 = -(m._01 = sin(angle)); return m;}
IT  IT::scaling   (real x, real y, real z) {auto m = MATRIX33_IDENT; m._00 = x; m._11 = y; m._22 = z; return m;}
IT  IT::scaling   (real scale)             {return IT::scaling(scale, scale, scale);}
IT& IT::operator+=(IT const& m)            {_00 += m._00; _01 += m._01; _02 += m._02; _10 += m._10; _11 += m._11; _12 += m._12; _20 += m._20; _21 += m._21; _22 += m._22; return self;}
IT& IT::operator-=(IT const& m)            {_00 -= m._00; _01 -= m._01; _02 -= m._02; _10 -= m._10; _11 -= m._11; _12 -= m._12; _20 -= m._20; _21 -= m._21; _22 -= m._22; return self;}
IT& IT::operator*=(real s)                 {_00 *= s;     _01 *= s;     _02 *= s;     _10 *= s;     _11 *= s;     _12 *= s;     _20 *= s;     _21 *= s;     _22 *= s;     return self;}
IT& IT::operator/=(real s)                 {_00 /= s;     _01 /= s;     _02 /= s;     _10 /= s;     _11 /= s;     _12 /= s;     _20 /= s;     _21 /= s;     _22 /= s;     return self;}

IT IT::operator-() const
{
        return IT(-_00, -_01, -_02,
                  -_10, -_11, -_12,
                  -_20, -_21, -_22);
}

IT IT::operator+ (IT const& m) const {return IT(_00 + m._00, _01 + m._01, _02 + m._02, _10 + m._10, _11 + m._11, _12 + m._12, _20 + m._20, _21 + m._21, _22 + m._22);} 
IT IT::operator- (IT const& m) const {return IT(_00 - m._00, _01 - m._01, _02 - m._02, _10 - m._10, _11 - m._11, _12 - m._12, _20 - m._20, _21 - m._21, _22 - m._22);}

IT IT::operator* (real s) const
{
        return IT(_00 * s, _01 * s, _02 * s,
                  _10 * s, _11 * s, _12 * s,
                  _20 * s, _21 * s, _22 * s);
}

IT IT::operator/ (real s) const {s = 1 / s; return self * s;}

void IT::transpose()
{
        swap(_01, _10); swap(_02, _20);
        swap(_12, _21);
}

IT IT::get_transpose() const
{
        return IT(_00, _10, _20,
                  _01, _11, _21,
                  _02, _12, _22);
}

IT IT::get_inverse() const // TODO this can be made more efficient by remembering some values computed by det
{
        real d = det();
        if( !d ) throw Zerodet("MatrixNM::get_inverse");
        d = 1 / d;
        return IT((_22*_11 - _21*_12) * d,  (_21*_02 - _22*_01) * d,  (_12*_01 - _11*_02) * d,
                  (_20*_12 - _22*_10) * d,  (_22*_00 - _20*_02) * d,  (_10*_02 - _12*_00) * d,
                  (_21*_10 - _20*_11) * d,  (_20*_01 - _21*_00) * d,  (_11*_00 - _10*_01) * d);
}

void IT::invert() {self = get_inverse();}

IT IT::operator* (IT const& m) const
{
        return IT(_00*m._00 + _10*m._01 + _20*m._02,  _00*m._10 + _10*m._11 + _20*m._12,  _00*m._20 + _10*m._21 + _20*m._22,
                  _01*m._00 + _11*m._01 + _21*m._02,  _01*m._10 + _11*m._11 + _21*m._12,  _01*m._20 + _11*m._21 + _21*m._22, 
                  _02*m._00 + _12*m._01 + _22*m._02,  _02*m._10 + _12*m._11 + _22*m._12,  _02*m._20 + _12*m._21 + _22*m._22);
}

IT& IT::operator*=(IT const& m) {return self = self * m;}

IT::operator real*  ()       {return data[0];}
IT::operator realCP () const {return data[0];}

real IT::trace() const {return _00 + _11 + _22;}
real IT::det  () const {return _00*(_11*_22 - _12*_21) +
                               _01*(_12*_20 - _10*_22) +
                               _02*(_10*_21 - _11*_20);}

real& IT::operator()(uint r, uint c)       {assert(r  <3 && c  <3); return data[r]  [c];}
real  IT::operator()(uint r, uint c) const {assert(r  <3 && c  <3); return data[r]  [c];}
real& IT::operator()(Pt2u p)               {assert(p.x<3 && p.y<3); return data[p.x][p.y];}
real  IT::operator()(Pt2u p) const         {assert(p.x<3 && p.y<3); return data[p.x][p.y];}

#undef IT
#define IT Matrix34

   IT::IT(realCP s) {memcpy(data, s, sizeof(IT));} 
   IT::IT(Matrix44CR m)                                                                                                           : _00(m._00), _01(m._01), _02(m._02), _10(m._10), _11(m._11), _12(m._12), _20(m._20), _21(m._21), _22(m._22), _30(m._30), _31(m._31), _32(m._32) {} 
   IT::IT(Matrix33CR m)                                                                                                           : _00(m._00), _01(m._01), _02(m._02), _10(m._10), _11(m._11), _12(m._12), _20(m._20), _21(m._21), _22(m._22), _30(0),     _31(0),     _32(0)     {} 
   IT::IT(real _00, real _01, real _02, real _10, real _11, real _12, real _20, real _21, real _22, real _30, real _31, real _32) : _00(_00),   _01(_01),   _02(_02),   _10(_10),   _11(_11),   _12(_12),   _20(_20),   _21(_21),   _22(_22),   _30(_30),   _31(_31),   _32(_32)   {} 
IT IT::scaling   (real x, real y, real z) {auto m = MATRIX34_IDENT; m._00 = x; m._11 = y; m._22 = z; return m;}
IT IT::scaling   (real scale)             {return IT::scaling(scale, scale, scale);}
IT IT::xrotation(real angle)             {auto m = MATRIX34_IDENT; m._11 = m._22 = cos(angle); m._21 = -(m._12 = sin(angle)); return m;}
IT IT::yrotation(real angle)             {auto m = MATRIX34_IDENT; m._00 = m._22 = cos(angle); m._02 = -(m._20 = sin(angle)); return m;}
IT IT::zrotation(real angle)             {auto m = MATRIX34_IDENT; m._00 = m._11 = cos(angle); m._10 = -(m._01 = sin(angle)); return m;}

     IT::operator real*  ()       {return data[0];}
     IT::operator realCP () const {return data[0];} 
real IT::trace           () const {return _00 + _11 + _22 + 1;}

IT IT::operator*(IT const& o) const
{
        return IT(o._00*_00 + o._10*_01 + o._20*_02,         o._01*_00 + o._11*_01 + o._21*_02,         o._02*_00 + o._12*_01 + o._22*_02,
                  o._00*_10 + o._10*_11 + o._20*_12,         o._01*_10 + o._11*_11 + o._21*_12,         o._02*_10 + o._12*_11 + o._22*_12,
                  o._00*_20 + o._10*_21 + o._20*_22,         o._01*_20 + o._11*_21 + o._21*_22,         o._02*_20 + o._12*_21 + o._22*_22,
                  o._00*_30 + o._10*_31 + o._20*_32 + o._30, o._01*_30 + o._11*_31 + o._21*_32 + o._31, o._02*_30 + o._12*_31 + o._22*_32 + o._32);
}

IT& IT::operator*=(IT const& m) {return self = self * m;}

real IT::det() const
{
        return _00*(_11*_22 - _21*_12) 
             + _10*(_21*_02 - _01*_22)
             + _20*(_01*_12 - _11*_02);
}

IT IT::translation(real x, real y, real z) {auto m = MATRIX34_IDENT; m._30 = x; m._31 = y; m._32 = z; return m;}

IT IT::orthographicLH(real w, real h, real n, real f) {                 return IT(2/w, 0, 0, 0, 2/h, 0, 0, 0, 1/(f-n), 0, 0, n/(n-f));} 
IT IT::orthographicRH(real w, real h, real n, real f) {real fn = n - f; return IT(2/w, 0, 0, 0, 2/h, 0, 0, 0, 1/fn,    0, 0, n/fn);}

IT IT::orthographic_offcenterLH(real l, real r, real b, real t, real n, real f) {                 return IT(2/(r-l), 0, 0, 0, 2/(t-b), 0, 0, 0, 1/(f-n), (l+r)/(l-r),  (t+b)/(b-t),  n/(n-f));}
IT IT::orthographic_offcenterRH(real l, real r, real b, real t, real n, real f) {real fn = n - f; return IT(2/(r-l), 0, 0, 0, 2/(t-b), 0, 0, 0, 1/fn,    (l+r)/(l-r),  (t+b)/(b-t),  n/fn);}

IT IT::get_inverse() const // TODO this can be made more efficient by remembering terms computed by det
{
        real d = det();
        if( !d ) throw Zerodet("MatrixNM::get_invese");
        d = 1 / d;
        return IT((-_12*_21+_11*_22) * d,                                                         ( _02*_21-_01*_22) * d,                                                         (-_02*_11+_01*_12) * d,
                  ( _12*_20-_10*_22) * d,                                                         (-_02*_20+_00*_22) * d,                                                         ( _02*_10-_00*_12) * d,
                  (-_11*_20+_10*_21) * d,                                                         ( _01*_20-_00*_21) * d,                                                         (-_01*_10+_00*_11) * d,
                  ( _12*_21*_30-_11*_22*_30-_12*_20*_31+_10*_22*_31+_11*_20*_32-_10*_21*_32) * d, (-_02*_21*_30+_01*_22*_30+_02*_20*_31-_00*_22*_31-_01*_20*_32+_00*_21*_32) * d, ( _02*_11*_30-_01*_12*_30-_02*_10*_31+_00*_12*_31+_01*_10*_32-_00*_11*_32) * d);
}

void IT::invert() {self = get_inverse();}

IT operator *(Matrix33CR a, IT const& b) {return IT(b._00*a._00 + b._10*a._01 + b._20*a._02, b._01*a._00 + b._11*a._01 + b._21*a._02, b._02*a._00 + b._12*a._01 + b._22*a._02, b._00*a._10 + b._10*a._11 + b._20*a._12, b._01*a._10 + b._11*a._11 + b._21*a._12, b._02*a._10 + b._12*a._11 + b._22*a._12, b._00*a._20 + b._10*a._21 + b._20*a._22, b._01*a._20 + b._11*a._21 + b._21*a._22, b._02*a._20 + b._12*a._21 + b._22*a._22, b._30,                                   b._31,                                   b._32);}
IT operator *(IT const& a, Matrix33CR b) {return IT(b._00*a._00 + b._10*a._01 + b._20*a._02, b._01*a._00 + b._11*a._01 + b._21*a._02, b._02*a._00 + b._12*a._01 + b._22*a._02, b._00*a._10 + b._10*a._11 + b._20*a._12, b._01*a._10 + b._11*a._11 + b._21*a._12, b._02*a._10 + b._12*a._11 + b._22*a._12, b._00*a._20 + b._10*a._21 + b._20*a._22, b._01*a._20 + b._11*a._21 + b._21*a._22, b._02*a._20 + b._12*a._21 + b._22*a._22, b._00*a._30 + b._10*a._31 + b._20*a._32, b._01*a._30 + b._11*a._31 + b._21*a._32, b._02*a._30 + b._12*a._31 + b._22*a._32);}

#undef IT
#define IT Matrix44

IT::IT(realCP s) {memcpy(data, s, sizeof(IT));}

IT::IT(Matrix22CR m)                                                                                                                                                   : _00(m._00), _01(m._01), _02(0),     _03(0),   _10(m._10), _11(m._11), _12(0),     _13(0),   _20(0),     _21(0),     _22(0),     _23(0),   _30(0),     _31(0),     _32(0),     _33(1)   {}
IT::IT(Matrix23CR m)                                                                                                                                                   : _00(m._00), _01(m._01), _02(0),     _03(0),   _10(m._10), _11(m._11), _12(0),     _13(0),   _20(m._20), _21(m._21), _22(0),     _23(0),   _30(0),     _31(0),     _32(0),     _33(1)   {}
IT::IT(Matrix33CR m)                                                                                                                                                   : _00(m._00), _01(m._01), _02(m._02), _03(0),   _10(m._10), _11(m._11), _12(m._12), _13(0),   _20(m._20), _21(m._21), _22(m._22), _23(0),   _30(0),     _31(0),     _32(0),     _33(1)   {}
IT::IT(Matrix34CR m)                                                                                                                                                   : _00(m._00), _01(m._01), _02(m._02), _03(0),   _10(m._10), _11(m._11), _12(m._12), _13(0),   _20(m._20), _21(m._21), _22(m._22), _23(0),   _30(m._30), _31(m._31), _32(m._32), _33(1)   {}
IT::IT(real _00, real _01, real _02, real _03, real _10, real _11, real _12, real _13, real _20, real _21, real _22, real _23, real _30, real _31, real _32, real _33) : _00(_00),   _01(_01),   _02(_02),   _03(_03), _10(_10),   _11(_11),   _12(_12),   _13(_13), _20(_20),   _21(_21),   _22(_22),   _23(_23), _30(_30),   _31(_31),   _32(_32),   _33(_33) {}

IT IT::perspectiveLH(real w, real h, real n, real f) {                     return IT(2*n/w, 0, 0, 0, 0, 2*n/h, 0, 0, 0, 0, f/(f-n), 1, 0, 0, n*f/(n-f), 0);} 
IT IT::perspectiveRH(real w, real h, real n, real f) {real fn = f / (n-f); return IT(2*n/w, 0, 0, 0, 0, 2*n/h, 0, 0, 0, 0, fn,     -1, 0, 0, fn * n,  0);}

IT IT::perspective_fovLH(real fovy, real aspect, real n, real f) {real yscale = cot(fovy / 2); real xscale = yscale / aspect;                        return IT(xscale, 0, 0, 0, 0, yscale, 0, 0, 0, 0, f/(f-n), 1, 0, 0, n*f/(n-f), 0);} 
IT IT::perspective_fovRH(real fovy, real aspect, real n, real f) {real yscale = cot(fovy / 2); real xscale = yscale / aspect; real fn = f / (n - f); return IT(xscale, 0, 0, 0, 0, yscale, 0, 0, 0, 0, fn,     -1, 0, 0, fn * n,    0);}

IT IT::perspective_offcenterLH(real l, real r, real b, real t, real n, real f) {                       return IT(2*n/(r-l), 0, 0, 0, 0, 2*n/(t-b), 0, 0, (l+r)/(l-r), (t+b)/(b-t), f/(f-n),  1, 0, 0, n*f/(n-f), 0);} 
IT IT::perspective_offcenterRH(real l, real r, real b, real t, real n, real f) {real fn = f / (n - f); return IT(2*n/(r-l), 0, 0, 0, 0, 2*n/(t-b), 0, 0, (l+r)/(r-l), (t+b)/(t-b), fn,      -1, 0, 0, fn * n,    0);}

IT& IT::operator+=(IT const& m) {DO_TIMES(r, 4) DO_TIMES(c, 4) data[r][c] += m.data[r][c]; return self;}
IT& IT::operator-=(IT const& m) {DO_TIMES(r, 4) DO_TIMES(c, 4) data[r][c] -= m.data[r][c]; return self;}
IT& IT::operator*=(real s)      {DO_TIMES(r, 4) DO_TIMES(c, 4) data[r][c] *= s;            return self;}
IT& IT::operator/=(real s)      {DO_TIMES(r, 4) DO_TIMES(c, 4) data[r][c] /= s;            return self;}

IT IT::operator*(IT const& o) const
{
        return IT(o._00*_00 + o._10*_01 + o._20*_02 + o._30*_03, o._01*_00 + o._11*_01 + o._21*_02 + o._31*_03, o._02*_00 + o._12*_01 + o._22*_02 + o._32*_03, o._03*_00 + o._13*_01 + o._23*_02 + o._33*_03,
                  o._00*_10 + o._10*_11 + o._20*_12 + o._30*_13, o._01*_10 + o._11*_11 + o._21*_12 + o._31*_13, o._02*_10 + o._12*_11 + o._22*_12 + o._32*_13, o._03*_10 + o._13*_11 + o._23*_12 + o._33*_13,
                  o._00*_20 + o._10*_21 + o._20*_22 + o._30*_23, o._01*_20 + o._11*_21 + o._21*_22 + o._31*_23, o._02*_20 + o._12*_21 + o._22*_22 + o._32*_23, o._03*_20 + o._13*_21 + o._23*_22 + o._33*_23,
                  o._00*_30 + o._10*_31 + o._20*_32 + o._30*_33, o._01*_30 + o._11*_31 + o._21*_32 + o._31*_33, o._02*_30 + o._12*_31 + o._22*_32 + o._32*_33, o._03*_30 + o._13*_31 + o._23*_32 + o._33*_33);
}

IT& IT::operator*=   (IT const& m) {return self = self * m;} 

void IT::invert          ()       {self = get_inverse();} 
     IT::operator real*  ()       {return data[0];}
     IT::operator realCP () const {return data[0];}

real IT::trace() const {return _00 + _11 + _22 + _33;}

real IT::det() const
{
        real _0213 = _02 * _13, _0223 = _02 * _23, _0233 = _02 * _33;
        real _1203 = _12 * _03, _1223 = _12 * _23, _1233 = _12 * _33;
        real _2203 = _22 * _03, _2213 = _22 * _13, _2233 = _22 * _33;
        real _3203 = _32 * _03, _3213 = _32 * _13, _3223 = _32 * _23;

        return _00*(_11*(_2233 - _3223) + _31*(_1223 - _2213) + _21*(_3213 - _1233)) 
             + _10*(_01*(_3223 - _2233) + _21*(_0233 - _3203) + _31*(_2203 - _0223))
             + _20*(_01*(_1233 - _3213) + _11*(_3203 - _0233) + _31*(_0213 - _1203))
             + _30*(_01*(_2213 - _1223) + _11*(_0223 - _2203) + _21*(_1203 - _0213));
}

real& IT::operator()(uint r, uint c)       {assert(r  <4 && c  <4); return data[r]  [c];}
real  IT::operator()(uint r, uint c) const {assert(r  <4 && c  <4); return data[r]  [c];}
real& IT::operator()(Pt2u p)               {assert(p.x<4 && p.y<4); return data[p.x][p.y];}
real  IT::operator()(Pt2u p) const         {assert(p.x<4 && p.y<4); return data[p.x][p.y];}

IT operator*(Matrix34CR a, IT const& b)
{
        return IT(b._00*a._00 + b._10*a._01 + b._20*a._02,         b._01*a._00 + b._11*a._01 + b._21*a._02,         b._02*a._00 + b._12*a._01 + b._22*a._02,         b._03*a._00 + b._13*a._01 + b._23*a._02,
                  b._00*a._10 + b._10*a._11 + b._20*a._12,         b._01*a._10 + b._11*a._11 + b._21*a._12,         b._02*a._10 + b._12*a._11 + b._22*a._12,         b._03*a._10 + b._13*a._11 + b._23*a._12,
                  b._00*a._20 + b._10*a._21 + b._20*a._22,         b._01*a._20 + b._11*a._21 + b._21*a._22,         b._02*a._20 + b._12*a._21 + b._22*a._22,         b._03*a._20 + b._13*a._21 + b._23*a._22,
                  b._00*a._30 + b._10*a._31 + b._20*a._32 + b._30, b._01*a._30 + b._11*a._31 + b._21*a._32 + b._31, b._02*a._30 + b._12*a._31 + b._22*a._32 + b._32, b._03*a._30 + b._13*a._31 + b._23*a._32 + b._33);
}

IT  IT::get_transpose() const      {return IT(_00, _10, _20, _30, _01, _11, _21, _31, _02, _12, _22, _32, _03, _13, _23, _33);}

void IT::transpose()
{
        swap(_01, _10); swap(_02, _20); swap(_03, _30);
        swap(_12, _21); swap(_13, _31);
        swap(_23, _32);
}

IT IT::get_inverse() const
{
        real d = det();
        if( !d ) throw Zerodet("MatrixNM::get_inverse");
        fail;
        return self;
}
