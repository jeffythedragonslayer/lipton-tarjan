#pragma once
#include "mathutil.h"

struct Vec2;
typedef Vec2 Pt2;
typedef Pt2 const* Pt2CP;
typedef Pt2 const& Pt2CR;

struct Vec2
{
        real x, y;

        operator        real*                 ()          {return &x;};
        operator const  real*                 ()    const {return &x;};

                        Vec2                  ()                                        {};
               explicit Vec2                  (realCP data)    : x(data[0]), y(data[1]) {}
                        Vec2                  (real x, real y) : x(x),       y(y)       {}
        static Vec2     random_unit_circle    ();
        static Vec2     random_unit_semicircle(Vec2CR n);
        static Vec2     random_unit_square    ();
        static Vec2     hermite               (Pt2CR pos1, Vec2CR tan1, Pt2CR pos2, Vec2CR tan2, real s);

        bool            operator==         (Vec2CR o) const   {return !memcmp(this, &o, sizeof(Vec2));}
        Vec2&           operator+=         (Vec2CR);
        Vec2&           operator-=         (Vec2CR);
        Vec2&           operator*=         (Vec2CR);
        Vec2&           operator/=         (Vec2CR);
        Vec2&           operator*=         (real);
        Vec2&           operator/=         (real);

        Vec2            operator+          ()         const {return self;};
        Vec2            operator-          ()         const {return Vec2(-x, -y);}
        Vec2&           invert             ()               {return self = -self;}

        Vec2            operator+          (Vec2CR) const;
        Vec2            operator-          (Vec2CR) const;
        //Vec2            operator*          (Vec2CR) const;
        //Vec2            operator/          (Vec2CR) const;
        Vec2            operator*          (real)   const;
        Vec2            operator/          (real)   const;
        friend Vec2     operator*          (real, Vec2CR);
        friend Vec2     operator/          (real, Vec2CR);

        bool            operator<          (Vec2CR) const;

        real            length_sqr         ()         const {return x*x + y*y;}
        real            length             ()         const;
        bool            is_zero            ()         const {return !(x || y);}
        bool            is_unit            (real tol) const;
        bool            is_real            ()         const   {return ::is_real(x) && ::is_real(y);}
        Vec2&           set_length         (real);

        real            dot                (Vec2CR v) const   {return x*v.x + y*v.y;}
        Vec2            component_product  (Vec2CR) const;
        Vec2            component_quotient (Vec2CR) const;

        Vec2             normal            ()       const;
        Vec2            znormal            ()       const;
        Vec2&           normalize          ();
        Vec2            minimum            (Vec2CR) const;
        Vec2            maximum            (Vec2CR) const;
        Vec2&           minimize           (Vec2CR);
        Vec2&           maximize           (Vec2CR);

        real            angle              ()       const;

        real            perp_dot           (Vec2CR) const;  // signed area of parallelogram formed by self and b
        Vec2            perp_ccw           ()       const {return Vec2(-y, x);}
        Vec2            perp_cw            ()       const {return Vec2(y, -x);}

        real            sqr_dist_to        (Pt2CR)  const;
        real            dist_to            (Pt2CR)  const;
        
        Vec2            proj               (Vec2CR) const;
        Vec2            abs                ()       const {return Vec2(std::abs(x), std::abs(y));}
};

const Vec2 VEC2_ZERO (  0,   0);
const Vec2 VEC2_ONE  (  1,   1);
const Vec2 VEC2_INF  (INF, INF);
const Vec2 VEC2_UP   (  0,   1);
const Vec2 VEC2_DOWN (  0,  -1);
const Vec2 VEC2_RIGHT(  1,   0);
const Vec2 VEC2_LEFT ( -1,   0);

struct Vec2i
{
        int x, y;

        Vec2i() {}
        Vec2i(int x, int y) : x(x), y(y) {}
};

typedef Vec2i Pt2i;
typedef Pt2i const* Pt2iCP;
typedef Pt2i const& Pt2iCR;

struct Vec2u
{
        uint x, y;

        Vec2u() {};
        Vec2u(uint x, uint y) : x(x), y(y) {}
};

const Vec2u VEC2U_ZERO(0, 0);
const Vec2u VEC2U_ONE (1, 1);

typedef Vec2u Pt2u;
typedef Pt2u const* Pt2uCP;
typedef Pt2u const& Pt2uCR;

struct Vec3;
typedef Vec3 Pt3;
typedef Pt3 const* Pt3CP;
typedef Pt3 const& Pt3CR;

struct Vec3
{
        real x, y, z; // assumes w is 1 when multiplying by Matrix34 or Matrix44

        operator real*                    ()                         {return &x;};
        operator realCP                   ()                   const {return &x;};

                    Vec3                  ()                                                            {};
        explicit    Vec3                  (realCP data)            : x(data[0]), y(data[1]), z(data[2]) {}
                    Vec3                  (real x, real y, real z) : x(x),       y(y),       z(z)       {}
        static Vec3 random_unit_sphere    ();
        static Vec3 random_unit_hemisphere(Vec3CR n);
        static Vec3 random_unit_cube      ();

        Vec3&       operator+=            (Vec3CR);
        Vec3&       operator-=            (Vec3CR);
        //Vec3&       operator*=          (Vec3CR);
        //Vec3&       operator/=          (Vec3CR);
        Vec3&       operator*=            (real);
        Vec3&       operator/=            (real);
        Vec3        operator-             ()                   const {return Vec3(-x, -y, -z);} 
        Vec3        operator+             ()                   const {return self;}
        Vec3&       invert                ()                         {return self = -self;}

        Vec3        operator+           (Vec3CR)             const;
        Vec3        operator-           (Vec3CR)             const;
        //Vec3        operator*           (Vec3CR)           const;
        //Vec3        operator/           (Vec3CR)           const;
        Vec3        operator*           (real)             const;
        Vec3        operator/           (real)             const;
        friend Vec3 operator*           (real, Vec3CR);
        friend Vec3 operator/           (real, Vec3CR);

        bool        operator==          (Vec3CR o)           const {return !memcmp(this, &o, sizeof(Vec3));}
        bool        operator<           (Vec3CR)             const;
         
        real        length_sqr          ()                   const {return x*x + y*y + z*z;}
        real        length              ()                   const;
        bool        is_zero             ()                   const {return !(x || y || z);}
        bool        is_unit             (real tol)           const;
        bool        is_real             ()                   const {return ::is_real(x) && ::is_real(y) && ::is_real(z);}
        Vec3&       set_length          (real);

        real        dot                 (Vec3CR v)           const {return x*v.x + y*v.y + z*v.z;}
        Vec3        cross               (Vec3CR)             const;
        real        box                 (Vec3CR v, Vec3CR w) const;
        Vec3        component_product   (Vec3CR)             const;
        Vec3        component_quotient  (Vec3CR)             const;

        Vec3         normal             ()                   const;
        Vec3        znormal             ()                   const;
        Vec3&       normalize           ();
        Vec3        minimum             (Vec3CR)             const;
        Vec3        maximum             (Vec3CR)             const;
        Vec3&       minimize            (Vec3CR);
        Vec3&       maximize            (Vec3CR);
        real        dist_to             (Vec3CR)             const;
        real        sqr_dist_to         (Vec3CR)             const;
        Vec3        proj                (Vec3CR)             const;
        Vec3        abs                 ()                   const {return Vec3(std::abs(x), std::abs(y), std::abs(z));} 
};

namespace std
{
        template<> struct hash<Vec3>
        {
                size_t operator() (Vec3CR v) const
                {
                        uint p[] = {73856093, 19349663, 83492791};
                        uint a = v.x;
                        uint b = v.y;
                        uint c = v.z;
                        return (a*p[0]) ^ (b*p[1]) ^ (c*p[2]);
                }
        };
}

bool make_orthonormal_basis(Vec3CR x, Vec3& y, Vec3& z);

const Vec3 VEC3_ZERO     (  0,   0,   0);
const Vec3 VEC3_ONE      (  1,   1,   1);
const Vec3 VEC3_INF      (INF, INF, INF);
const Vec3 VEC3_UP       (  0,   1,   0);
const Vec3 VEC3_DOWN     (  0,  -1,   0);
const Vec3 VEC3_LEFT     ( -1,   0,   0);
const Vec3 VEC3_RIGHT    (  1,   0,   0);
const Vec3 VEC3_FORWARD  (  0,   0,   1);
const Vec3 VEC3_BACKWARDS(  0,   0,  -1);

struct Vec3i
{
        int x, y, z;
};

typedef Vec3i Pt3i;
typedef Pt3i const* Pt3iCP;
typedef Pt3i const& Pt3iCR;

struct Vec3u
{
        uint x, y, z;
};

typedef Vec3u Pt3u;
typedef Pt3u const* Pt3uCP;
typedef Pt3u const& Pt3uCR;

struct Vec4;
typedef Vec4 Pt4;
typedef Pt4 const* Pt4CP;
typedef Pt4 const& Pt4CR;

struct Vec4
{
        real x, y, z, w;

        operator real*                 ()               {return &x;};
        operator realCP                ()         const {return &x;};

                    Vec4               ()                                                                                {};
        explicit    Vec4               (realCP data)                    : x(data[0]), y(data[1]), z(data[2]), w(data[2]) {}
                    Vec4               (real x, real y, real z, real w) : x(x),       y(y),       z(z),       w(w)       {}
        static Vec4 random_unit_hypersphere    ();
        static Vec4 random_unit_semihypersphere(Vec4CR n);
        static Vec4 random_unit_tesseract      ();

        Vec4&       operator+=         (Vec4CR);
        Vec4&       operator-=         (Vec4CR);
        //Vec4&       operator*=         (Vec4CR);
        //Vec4&       operator/=         (Vec4CR);
        Vec4&       operator*=         (real);
        Vec4&       operator/=         (real);

        Vec4        operator+          ()         const {return self;}
        Vec4        operator-          ()         const {return Vec4(-x, -y, -z, -w);}
        Vec4&       invert             ()               {return self = -self;}

        Vec4        operator+          (Vec4CR) const;
        Vec4        operator-          (Vec4CR) const;
        //Vec4        operator*          (Vec4CR) const;
        //Vec4        operator/          (Vec4CR) const;
        Vec4        operator*          (real)   const;
        Vec4        operator/          (real)   const;
        friend Vec4 operator*          (real, Vec4CR);
        friend Vec4 operator/          (real, Vec4CR);
        bool        operator==         (Vec4CR o)           const {return !memcmp(this, &o, sizeof(Vec4));}
        bool        operator<          (Vec4CR) const;

        real        length_sqr         ()         const {return x*x + y*y + z*z + w*w;}
        real        length             ()         const;
        bool        is_zero            ()         const {return !(x || y || z || w);}
        bool        is_unit            (real tol) const;
        bool        is_real            ()         const {return ::is_real(x) && ::is_real(y) && ::is_real(z) && ::is_real(w);}
        Vec4&       set_length         (real);

        real        dot                (Vec4CR v) const {return x*v.x + y*v.y + z*v.z + w*v.w;}
        Vec4        component_product  (Vec4CR) const;
        Vec4        component_quotient (Vec4CR) const;
        
        Vec4         normal            ()       const;
        Vec4        znormal            ()       const;
        Vec4&       normalize          ();
        Vec4        minimum            (Vec4CR) const;
        Vec4        maximum            (Vec4CR) const;
        Vec4&       minimize           (Vec4CR);
        Vec4&       maximize           (Vec4CR);
        real        sqr_dist_to        (Pt4CR)  const;
        real        dist_to            (Pt4CR)  const;
        Vec4        proj               (Vec4CR) const;
        Vec4        abs                ()       const {return Vec4(std::abs(x), std::abs(y), std::abs(z), std::abs(w));}
};

const Vec4 VEC4_ZERO  (  0,   0,   0,   0);
const Vec4 VEC4_ORIGIN(  0,   0,   0,   1);
const Vec4 VEC4_ONE   (  1,   1,   1,   1);
const Vec4 VEC4_INF   (INF, INF, INF, INF);
const Vec4 VEC4_UP    (  0,   1,   0,   0);

struct Vec4i
{
        int x, y, z, w;
};

typedef Vec4i Pt4i;
typedef Pt4i const* Pt4iCP;
typedef Pt4i const& Pt4iCR;

struct Vec4u
{
        uint x, y, z, w;
};

typedef Vec4u Pt4u;
typedef Pt4u const* Pt4uCP;
typedef Pt4u const& Pt4uCR;
