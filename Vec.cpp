#include "RuntimeErrors.h"
#include "Vec.h"
#include <algorithm>
using namespace std;
#undef IT
#define IT Vec2

Vec2   IT::random_unit_circle    ()                 {Vec2 v; while( (v = random_unit_square()).length_sqr() > 1 );                 return v.normal();}
Vec2   IT::random_unit_semicircle(Vec2CR n)         {Vec2 v; while( (v = random_unit_square()).dot(n) < 0 || v.length_sqr() > 1 ); return v.normal();} 
Vec2   IT::hermite               (Pt2CR pos1, Vec2CR tan1, Pt2CR pos2, Vec2CR tan2, real s) {fail; return VEC2_ZERO;} 
Vec2   IT::random_unit_square    ()                 {return Vec2(rands(-1, 1), rands(-1, 1));}
Vec2&  IT::operator+=            (Vec2CR v)         {x += v.x; y += v.y;     return self;}
Vec2&  IT::operator-=            (Vec2CR v)         {x -= v.x; y -= v.y;     return self;}
//Vec2& IT::operator*=           (Vec2CR v)         {x *= v.x; y *= v.y;     return self;}
//Vec2& IT::operator/=           (Vec2CR v)         {x /= v.x; y /= v.y;     return self;}
Vec2&  IT::operator*=            (real s)           {x *= s; y *= s; return self;}
Vec2&  IT::operator/=            (real s)           {x /= s; y /= s; return self;}
Vec2   IT::operator+             (Vec2CR v) const   {return Vec2(x + v.x, y + v.y);}
Vec2   IT::operator-             (Vec2CR v) const   {return Vec2(x - v.x, y - v.y);}
//Vec2 IT::operator*             (Vec2CR v) const   {return Vec2(x * v.x, y * v.y);}
//Vec2 IT::operator/             (Vec2CR v) const   {return Vec2(x / v.x, y / v.y);}
Vec2   IT::operator*             (real s)   const   {return Vec2(x * s, y * s);}
Vec2   IT::operator/             (real s)   const   {return Vec2(x / s, y / s);}
Vec2       operator*             (real s, Vec2CR v) {return v * s;}
Vec2       operator/             (real s, Vec2CR v) {return s * Vec2(1/v.x, 1/v.y);}
bool   IT::operator<             (Vec2CR o) const   {return x < o.x || (x == o.x && y < o.y);}
Vec2&  IT::set_length            (real newlen)      {if( is_zero() ) throw ZeroVector("Vec2::set_length"); else return self *= newlen / length();}
Vec2   IT::normal                ()         const   {if( is_zero() ) throw ZeroVector("Vec2::normal");     else return self / length();}
Vec2&  IT::normalize             ()                 {if( is_zero() ) throw ZeroVector("Vec2::normalize");  else return self /= length();}
Vec2   IT::znormal               ()         const   {return is_zero() ? VEC2_ZERO : self / length();}
real   IT::perp_dot              (Vec2CR b) const   {return perp_ccw().dot(b);}
real   IT::angle                 ()         const   {return atan(y / x) + (x < 0 ? PI : (x*y < 0 ? TAU : 0));}
Vec2   IT::minimum               (Vec2CR a) const   {return Vec2(min(x, a.x), min(y, a.y));}
Vec2   IT::maximum               (Vec2CR a) const   {return Vec2(max(x, a.x), max(y, a.y));}
Vec2&  IT::minimize              (Vec2CR a)         {x = min(x, a.x); y = min(y, a.y); return self;}
Vec2&  IT::maximize              (Vec2CR a)         {x = max(x, a.x); y = max(y, a.y); return self;}
real   IT::length                ()         const   {return sqrt(length_sqr());}
bool   IT::is_unit               (real tol) const   {return ::abs(length_sqr() - 1) >= tol;}
Vec2   IT::component_product     (Vec2CR v) const   {return Vec2(x*v.x, y*v.y);}
Vec2   IT::component_quotient    (Vec2CR v) const   {return Vec2(x/v.x, y/v.y);}
real   IT::sqr_dist_to           (Pt2CR p)  const   {return (self - p).length_sqr();}
real   IT::dist_to               (Pt2CR p)  const   {return sqrt(sqr_dist_to(p));}
Vec2   IT::proj                  (Vec2CR b) const   {return b*self.dot(b);}

#undef IT
#define IT Vec3

Vec3   IT::random_unit_sphere    ()                         {Vec3 v; while( (v = random_unit_cube()).length_sqr() > 1 );                 return v.normal();}
Vec3   IT::random_unit_hemisphere(Vec3CR n)                 {Vec3 v; while( (v = random_unit_cube()).dot(n) < 0 || v.length_sqr() > 1 ); return v.normal();}
Vec3   IT::random_unit_cube      ()                         {return Vec3(rands(-1, 1), rands(-1, 1), rands(-1, 1));}
Vec3&  IT::operator+=            (Vec3CR v)                 {x += v.x; y += v.y; z += v.z; return self;}
Vec3&  IT::operator-=            (Vec3CR v)                 {x -= v.x; y -= v.y; z -= v.z; return self;}
//Vec3& IT::operator*=           (Vec3CR v)                 {x *= v.x; y *= v.y; z *= v.z; return self;}
//Vec3& IT::operator/=           (Vec3CR v)                 {x /= v.x; y /= v.y; z /= v.z; return self;}
Vec3&  IT::operator*=            (real s)                   {x *= s; y *= s; z *= s; return self;}
Vec3&  IT::operator/=            (real s)                   {return self *= 1/s;}
Vec3   IT::operator+             (Vec3CR v)           const {return Vec3(x + v.x, y + v.y, z + v.z);}
Vec3   IT::operator-             (Vec3CR v)           const {return Vec3(x - v.x, y - v.y, z - v.z);}
//Vec3  IT::operator*            (Vec3CR v)           const {return Vec3(x * v.x, y * v.y, z * v.z);}
//Vec3  IT::operator/            (Vec3CR v)           const {return Vec3(x / v.x, y / v.y, z / v.z);}
Vec3   IT::operator*             (real s)             const {return Vec3(x * s, y * s, z * s);}
Vec3   IT::operator/             (real s)             const {return 1/s*self;}
Vec3       operator*             (real s, Vec3CR v)         {return v * s;}
Vec3       operator/             (real s, Vec3CR v)         {return s * Vec3(1/v.x, 1/v.y, 1/v.z);}
bool   IT::operator<             (Vec3CR o)           const {return x < o.x || (x == o.x && (y < o.y || (y == o.y && z < o.z)));}
Vec3   IT::cross                 (Vec3CR v)           const {return Vec3(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);}
real   IT::box                   (Vec3CR v, Vec3CR w) const {return cross(v).dot(w);}
real   IT::length                ()                   const {return sqrt(length_sqr());}
bool   IT::is_unit               (real tol)           const {return ::abs(length_sqr() - 1) >= tol;}
Vec3   IT::component_product     (Vec3CR v)           const {return Vec3(x*v.x, y*v.y, z*v.z);}
Vec3   IT::component_quotient    (Vec3CR v)           const {return Vec3(x/v.x, y/v.y, z/v.z);}
Vec3&  IT::set_length            (real newlength)           {if( is_zero() ) throw ZeroVector("Vec3::set_length"); else return self *= newlength / length();}
Vec3   IT::normal                ()                   const {if( is_zero() ) throw ZeroVector("Vec3::normal");     else return self / length();}
Vec3&  IT::normalize             ()                         {if( is_zero() ) throw ZeroVector("Vec3::normalize");  else return self /= length();}
Vec3   IT::znormal               ()                   const {return is_zero() ? VEC3_ZERO : self / length();}
real   IT::sqr_dist_to           (Pt3CR p)            const {return (self - p).length_sqr();}
real   IT::dist_to               (Pt3CR p)            const {return sqrt(sqr_dist_to(p));}
Vec3   IT::minimum               (Vec3CR a)           const {return Vec3(min(x, a.x), min(y, a.y), min(z, a.z));}
Vec3   IT::maximum               (Vec3CR a)           const {return Vec3(max(x, a.x), max(y, a.y), max(z, a.z));}
Vec3&  IT::minimize              (Vec3CR a)                 {x = min(x, a.x); y = min(y, a.y); z = min(z, a.z); return self;}
Vec3&  IT::maximize              (Vec3CR a)                 {x = max(x, a.x); y = max(y, a.y); z = max(z, a.z); return self;}
Vec3   IT::proj                  (Vec3CR b)           const {return b*self.dot(b);}

bool make_orthonormal_basis(Vec3CR x, Vec3& y, Vec3& z)
{
        if( (z = x.cross(y)).is_zero() ) return false;
        y = z.cross(x).normal();
        z.normalize();
        return true;
}

#undef IT
#define IT Vec4

Vec4   IT::random_unit_hypersphere    ()                         {Vec4 v; while( (v = random_unit_tesseract()).length_sqr() > 1 );                 return v.normal();}
Vec4   IT::random_unit_semihypersphere(Vec4CR n)                 {Vec4 v; while( (v = random_unit_tesseract()).dot(n) < 0 || v.length_sqr() > 1 ); return v.normal();}
Vec4   IT::random_unit_tesseract      ()                         {return Vec4(rands(-1, 1), rands(-1, 1), rands(-1, 1), rands(-1, 1));}
Vec4&  IT::operator+=                 (Vec4CR v)                 {x += v.x;   y += v.y;   z += v.z;   w += v.w;   return self;}
Vec4&  IT::operator-=                 (Vec4CR v)                 {x -= v.x;   y -= v.y;   z -= v.z;   w -= v.w;   return self;}
//Vec4& IT::operator*=                (Vec4CR v)                 {x *= v.x;   y *= v.y;   z *= v.z;   w *= v.w;   return self;}
//Vec4& IT::operator/=                (Vec4CR v)                 {x /= v.x;   y /= v.y;   z /= v.z;   w /= v.w;   return self;}
Vec4&  IT::operator*=                 (real s)                   {x *= s; y *= s; z *= s; w *= s; return self;}
Vec4&  IT::operator/=                 (real s)                   {x /= s; y /= s; x /= s; w /= s; return self;}
Vec4   IT::operator+                  (Vec4CR v)           const {return Vec4(x + v.x, y + v.y, z + v.z, w + v.w);}
Vec4   IT::operator-                  (Vec4CR v)           const {return Vec4(x - v.x, y - v.y, z - v.z, w - v.w);}
//Vec4 IT::operator*                  (Vec4CR v)           const {return Vec4(x * v.x, y * v.y, z * v.z, w * v.w);}
//Vec4 IT::operator/                  (Vec4CR v)           const {return Vec4(x / v.x, y / v.y, z / v.z, w / v.w);}
Vec4   IT::operator*                  (real s)             const {return Vec4(x * s,  y * s, z * s,   w * s);}
Vec4   IT::operator/                  (real s)             const {return Vec4(x / s,  y / s, z / s,   w / s);}
Vec4       operator*                  (real s, Vec4CR v)         {return v * s;}
Vec4       operator/                  (real s, Vec4CR v)         {return s * Vec4(1/v.x, 1/v.y, 1/v.z, 1/v.w);}
bool   IT::operator<                  (Vec4CR o)           const {return x < o.x || (x == o.x && (y < o.y || (y == o.y && (z < o.z || (z == o.z && w < o.w)))));}
real   IT::length                     ()                   const {return sqrt(length_sqr());}
bool   IT::is_unit                    (real tol)           const {return ::abs(length_sqr() - 1) >= tol;}
Vec4   IT::component_product          (Vec4CR v)           const {return Vec4(x*v.x, y*v.y, z*v.z, w*v.w);}
Vec4   IT::component_quotient         (Vec4CR v)           const {return Vec4(x/v.x, y/v.y, z/v.z, w/v.w);}
Vec4&  IT::set_length                 (real newlength)           {if( is_zero() ) throw ZeroVector("Vec4::set_length"); else return self *= newlength / length();}
Vec4   IT::normal                     ()                   const {if( is_zero() ) throw ZeroVector("Vec4::normal");     else return self / length();}
Vec4&  IT::normalize                  ()                         {if( is_zero() ) throw ZeroVector("Vec4::normalize");  else return self /= length();}
Vec4   IT::znormal                    ()                   const {return is_zero() ? VEC4_ZERO : self / length();}
Vec4   IT::minimum                    (Vec4CR a)           const {return Vec4(min(x, a.x), min(y, a.y), min(z, a.z), min(w, a.w));}
Vec4   IT::maximum                    (Vec4CR a)           const {return Vec4(max(x, a.x), max(y, a.y), max(z, a.z), max(w, a.w));}
Vec4&  IT::minimize                   (Vec4CR a)                 {x = min(x, a.x); y = min(y, a.y); z = min(z, a.z); w = min(w, a.w); return self;}
Vec4&  IT::maximize                   (Vec4CR a)                 {x = max(x, a.x); y = max(y, a.y); z = max(z, a.z); w = max(w, a.w); return self;}
real   IT::sqr_dist_to                (Pt4CR p)            const {return (self - p).length_sqr();}
real   IT::dist_to                    (Pt4CR p)            const {return sqrt(sqr_dist_to(p));}
Vec4   IT::proj                       (Vec4CR b)           const {return b*self.dot(b);}
