#pragma once
#include "mathutil.h"
#include "Vec.h"

struct Matrix22
{
        union
        {
                struct
                {
                        real _00, _01;
                        real _10, _11;
                };
                real data[2][2];
        };

                 Matrix22() {};
        explicit Matrix22(realCP);
                 Matrix22(real, real,
                          real, real);

        static Matrix22 scaling(real, real);
        static Matrix22 scaling(real);

        operator real*  ();
        operator realCP () const;

        Matrix22& operator+=   (Matrix22CR);
        Matrix22& operator-=   (Matrix22CR);
        Matrix22& operator*=   (real);
        Matrix22& operator/=   (real);

        Matrix22  operator*    (Matrix22CR)      const;
        Matrix22& operator*=   (Matrix22CR);

        real&     operator()   (uint, uint);
        real      operator()   (uint, uint)      const;
        real&     operator()   (Pt2u);
        real      operator()   (Pt2u)            const;

        real      det          ()                const;
        real      trace        ()                const;
        real      rank         ()                const;

        void      transpose    ();
        Matrix22  get_transpose()                const;
        
        void      invert       ();
        Matrix22  get_inverse  ()                const;
};

struct Matrix23
{
        union
        {
                struct
                {
                        real _00, _01;
                        real _10, _11;
                        real _20, _21;
                };
                real data[3][2];
        };

                 Matrix23();
        explicit Matrix23(realCP);
        explicit Matrix23(Matrix33CR); // assumes last row is [0 0 0]
                 Matrix23(real, real,
                          real, real,
                          real, real);

        static Matrix23 translation(real, real);

        real      det                ()             const; // assume last column is [0 0 1]
        real      trace              ()             const; // assume last column is [0 0 1]
        real      rank               ()             const; // assume last column is [0 0 1]

        void      invert             ();
        Matrix34  get_inverse        ()             const;

};

struct Matrix33
{
        union
        {
                struct
                {
                        real _00, _01, _02;
                        real _10, _11, _12;
                        real _20, _21, _22;
                };
                real data[3][3];
        };

                 Matrix33() {};
        explicit Matrix33(realCP);
        explicit Matrix33(Matrix23CR);
        explicit Matrix33(Matrix34CR); // truncate additional data
        explicit Matrix33(Matrix44CR); // truncate additional data
                 Matrix33(real, real, real,
                          real, real, real,
                          real, real, real);

        static Matrix33 scaling   (real, real, real);
        static Matrix33 scaling   (real);
        static Matrix33 xrotation (real);
        static Matrix33 yrotation (real);
        static Matrix33 zrotation (real);

        operator real*  ();
        operator realCP () const;

        Matrix33& operator+=   (Matrix33CR);
        Matrix33& operator-=   (Matrix33CR);
        Matrix33& operator*=   (real);
        Matrix33& operator/=   (real);
        Matrix33  operator-    ()           const;
        Matrix33  operator+    (Matrix33CR) const;
        Matrix33  operator-    (Matrix33CR) const;
        Matrix33  operator*    (real)       const;
        Matrix33  operator/    (real)       const;
        
        Matrix33  operator*    (Matrix33CR) const;
        Matrix33& operator*=   (Matrix33CR);

        real&     operator()   (uint, uint);
        real      operator()   (uint, uint)  const;
        real&     operator()   (Pt2u);
        real      operator()   (Pt2u) const;

        real      det          ()            const;
        real      trace        ()            const;
        real      rank         ()            const;

        void      transpose    ();
        Matrix33  get_transpose()            const;
        
        void      invert       ();
        Matrix33  get_inverse  ()            const;
};

struct Matrix34
{
        union
        {
                struct
                {
                        real _00, _01, _02;
                        real _10, _11, _12;
                        real _20, _21, _22;
                        real _30, _31, _32;
                };
                real data[4][3];
        };

                 Matrix34              () {};
        explicit Matrix34              (realCP);
        explicit Matrix34              (Matrix33CR); // assumes last r is [0 0 0]
        explicit Matrix34              (Matrix44CR); // truncate additional data
                 Matrix34              (real, real, real,
                                        real, real, real,
                                        real, real, real,
                                        real, real, real);

        static   Matrix34 translation  (real x, real y, real z);
        static   Matrix34 xrotation    (real);
        static   Matrix34 yrotation    (real);
        static   Matrix34 zrotation    (real);
        static   Matrix34 scaling      (real, real, real);
        static   Matrix34 scaling      (real);
        static   Matrix34 orthographicLH          (real w, real h,                 real n, real f);
        static   Matrix34 orthographicRH          (real w, real h,                 real n, real f);
        static   Matrix34 orthographic_offcenterLH(real l, real r, real b, real t, real n, real f);
        static   Matrix34 orthographic_offcenterRH(real l, real r, real b, real t, real n, real f);

        operator        real* ();
        operator const  real* () const;

        Matrix34  operator*          (Matrix34CR) const;
        Matrix34& operator*=         (Matrix34CR);

        real      det                ()             const; // assume last column is [0 0 0 1]
        real      trace              ()             const; // assume last column is [0 0 0 1]
        real      rank               ()             const; // assume last column is [0 0 0 1]

        void      invert             ();
        Matrix34  get_inverse        ()             const;
};

struct Matrix44
{
        union
        {
                struct
                {
                        real  _00, _01, _02, _03;
                        real  _10, _11, _12, _13;
                        real  _20, _21, _22, _23;
                        real  _30, _31, _32, _33;
                };
                real data[4][4];
        };

                        Matrix44() {};
               explicit Matrix44(realCP);
               explicit Matrix44(Matrix22CR);
               explicit Matrix44(Matrix23CR);
               explicit Matrix44(Matrix33CR); // assumes last r is [0 0 0 0] and last column is [0 0 0 1]
               explicit Matrix44(Matrix34CR);
                        Matrix44(real, real, real, real,
                                 real, real, real, real,
                                 real, real, real, real,
                                 real, real, real, real);

        operator        real* ();
        operator const  real* ()                         const;

        static Matrix44 perspectiveLH          (real w,         real h,         real n, real f);
        static Matrix44 perspectiveRH          (real w,         real h,         real n, real f);
        static Matrix44 perspective_fovLH      (real fovy,      real aspect,    real n, real f);
        static Matrix44 perspective_fovRH      (real fovy,      real aspect,    real n, real f);
        static Matrix44 perspective_offcenterLH(real l, real r, real b, real t, real n, real f);
        static Matrix44 perspective_offcenterRH(real l, real r, real b, real t, real n, real f);

        real&           operator()    (uint, uint);
        real            operator()    (uint, uint) const;
        real&           operator()    (Pt2u);
        real            operator()    (Pt2u)       const;

        Matrix44&       operator*=    (Matrix44CR);
        Matrix44&       operator+=    (Matrix44CR);
        Matrix44&       operator-=    (Matrix44CR);
        Matrix44&       operator*=    (real);
        Matrix44&       operator/=    (real);

        Matrix44        operator+     ()           const;
        Matrix44        operator-     ()           const; 
        Matrix44        operator*     (Matrix44CR) const;
        Matrix44        operator+     (Matrix44CR) const;
        Matrix44        operator-     (Matrix44CR) const;
        Matrix44        operator*     (real)       const;
        Matrix44        operator/     (real)       const;
        friend Matrix44 operator*     (real, Matrix44&);

        real            det           ()           const;
        real            trace         ()           const;
        real            rank          ()           const;

        void            transpose     ();
        Matrix44        get_transpose ()           const;

        void            invert        ();
        Matrix44        get_inverse   ()           const;
};

Matrix34 operator* (Matrix33CR, Matrix34CR);
Matrix34 operator* (Matrix34CR, Matrix33CR);
Matrix44 operator* (Matrix34CR, Matrix44CR);

const Matrix22 MATRIX22_ZERO(0, 0,
                             0, 0);
        
const Matrix22 MATRIX22_IDENT(1, 0,
                              0, 1);

const Matrix22 MATRIX22_XREFLECT(-1, 0,
                                  0, 1);

const Matrix22 MATRIX22_YREFLECT(1,  0,
                                 0, -1);

const Matrix23 MATRIX23_ZERO(0, 0,
                             0, 0,
                             0, 0);
        
const Matrix23 MATRIX23_IDENT(1, 0,
                              0, 1,
                              0, 0);

const Matrix33 MATRIX33_ZERO(0, 0, 0,
                             0, 0, 0,
                             0, 0, 0);
        
const Matrix33 MATRIX33_IDENT(1, 0, 0,
                              0, 1, 0,
                              0, 0, 1);

const Matrix33 MATRIX33_XREFLECT(-1, 0, 0,
                                  0, 1, 0,
                                  0, 0, 0);

const Matrix33 MATRIX33_YREFLECT(1,  0, 0,
                                 0, -1, 0,
                                 0,  0, 1);

const Matrix33 MATRIX33_ZREFLECT(1, 0,  0,
                                 0, 1,  0,
                                 0, 0, -1);

const Matrix34 MATRIX34_ZERO(0, 0, 0,
                             0, 0, 0,
                             0, 0, 0,
                             0, 0, 0);
        
const Matrix34 MATRIX34_IDENT(1, 0, 0,
                              0, 1, 0,
                              0, 0, 1,
                              0, 0, 0);

const Matrix44 MATRIX44_ZERO(0, 0, 0, 0,
                             0, 0, 0, 0,
                             0, 0, 0, 0,
                             0, 0, 0, 0);
        
const Matrix44 MATRIX44_IDENT(1, 0, 0, 0,
                              0, 1, 0, 0,
                              0, 0, 1, 0,
                              0, 0, 0, 1);