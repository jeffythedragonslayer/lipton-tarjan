#include "stdafx.h"
#include "mathutil.h"
#include <cstdlib>
#include <algorithm>
using namespace std;

real abs_equals(real a, real b, real tol) {return abs(a - b) <= tol;}
real rel_equals(real a, real b, real tol) {return abs(a - b) <= tol * max<real>(abs(a), max<real>(abs(b), 1.));}

real rwa(real oldx, real newx, real bias) {return bias*oldx + (1-bias)*newx;}

real geometric_mean(real a, real b)       {return sqrt(a*b);}
real  harmonic_mean(real a, real b)       {return 2*a*b / (a+b);}

real wrapvalue(real x)                    {real r = fmod(x, TAU); return x > 0 ? r : -r;}

uint factorial(uint n)		          {return !n ? 1 : n*factorial(n-1);}

//real awayfromzero_round(real x)         {return x > 0 ? ceil(x) : floor(x);}

real csc      (real x)                    {return 1 / sin(x);}
real sec      (real x)                    {return 1 / cos(x);}
real cot      (real x)                    {return 1 / tan(x);}

real rands    ()                          {return rand()*1. / RAND_MAX;} // rts random real in [0, 1)
real rands    (real a, real b)            {return a + rands()*(b-a);}    // rts random real in [a, b)

real saturate (real x)                    {return x < 0 ? 0 : (x > 1 ? 1 : x);}
real sqr      (real x)                    {return x * x;}
real cube     (real x)                    {return x * x * x;}
real lerp     (real a, real b, real t)    {return a*(1-t) + b*t;}
real is_real  (real x)                    {return x > -INF && x < INF;}

uint ipow(uint b, uint e)
{
        assert(b || e);
        uint r = 1;
        while( e ){
                if (e & 1) r *= b;
                e >>= 1;
                b *= b;
        }
        return r;
}

bool is_pow_of_2(uint v) {return v > 0 && !(v & (v - 1));}
bool is_pow_of_2(int  v) {return v > 0 && !(v & (v - 1));}

uint log2_of_pow2 (uint pow2)
{
        uint log2  = !!(pow2 & 0xaaaaaaaa);
             log2 |= !!(pow2 & 0xffff0000) << 4;
             log2 |= !!(pow2 & 0xff00ff00) << 3;
             log2 |= !!(pow2 & 0xf0f0f0f0) << 2;
             log2 |= !!(pow2 & 0xcccccccc) << 1;
        return log2;
}

int log2_of_pow2 (int pow2)
{
        uint log2  = !!(pow2 & 0xaaaaaaaa);
             log2 |= !!(pow2 & 0xffff0000) << 4;
             log2 |= !!(pow2 & 0xff00ff00) << 3;
             log2 |= !!(pow2 & 0xf0f0f0f0) << 2;
             log2 |= !!(pow2 & 0xcccccccc) << 1;
        return static_cast<int>(log2);
}

int scale_FLOAT_to_int(float val, int pow)
{
        int shift = 150 - pow - ((*(int*)(&val) >> 23) & 0xFF);
        if( shift < 24 ){
                int r = ((*(int*)(&val) & 0x007fffff) | 0x00800000) >> shift;
                if( r == (1 << pow) ) --r;
                return r;
        } else return 0;
}

real cosine_interpolate(real a, real b, real t) 
{
        real f = (1 - cos(t * PI)) / 2;
        return a * (1 - f) + b*f;
}

real bezier_interpolate(real x0, real a, real b, real x3, real t)
{
        real t2  = t*t;
        real it  = 1 - t;
        real it2 = it * it;
        return x0*it2*it + 3*a*it2 + 3*b*it*t2 + x3*t2*t;
}

real cubic_interpolate(real x0, real a, real b, real x3, real t)
{
        real p  = x3 - b - x0 + a;
        real q  = x0 - a - p;
        real r  = b - x0;
        real t2 = t * t;
        return p*t2*t + q*t2 + r*t + a;
}

real catmullrom_interpolate(real x0, real a, real b, real x3, real t)
{
        real t2 = t * t;
        real a0 = 3*(a - b) + x3- x0;
        real a1 = 2*x0 - 5*a + 4*b - x3;
        real a2 = b - x0;
        return (a0*t2*t + a1*t2 + a2*t) / 2 + a;
}

real hermite_interpolate(real x0, real a, real b, real x3, real t, real tension, real bias) // Tension: 0 is high, -1 normal, -2 is low, Bias: 0 is even, positive is towards first segment, negative towards the other
{
        real m0 = (a-x0)*(1+bias) +  (b-a)*(1-bias);
        real m1 =  (b-a)*(1+bias) + (x3-b)*(1-bias);
        real t2 = t  * t;
        real t3 = t2 * t;
        real a0 = 2*t3 - 3*t2 + 1;
        real a1 =   t3 - 2*t2 + t;
        real a2 =   t3 -   t2;
        real a3 = 3*t2 - 2*t3;
        return (a0*a + (a1*m0 + a2*m1)*tension) / 2 + a3*b;
}

bool quadratic_roots(real& root1, real& root2, real a, real b, real c)
{
        real discr = b*b - 4 * a * c; if( discr < 0 ) return false; 
        real sqrt_d = sqrt(discr);
        root1       = (-b - sqrt_d) / (2 * a);
        root2       = (-b + sqrt_d) / (2 * a);
        return true;
}

bool higher_quadratic_root(real& root, real a, real b, real c)
{
        real r1, r2;
        bool has_root = quadratic_roots(r1, r2, a, b, c);
        root = max(r1, r2);
        return has_root;
}

bool lower_quadratic_root(real& root, real a, real b, real c)
{
        real r1, r2;
        bool has_root = quadratic_roots(r1, r2, a, b, c);
        root = min(r1, r2);
        return has_root;
}

bool solve_cubic(real& t, real& u, real& v, real e, real f, real g)
{
        real p  = f - e*e / 3;
        real q  = g + (e*e*e * 2 / 9 - e*f) / 3;
        real q2 = q*q;
        real h  = q2 / 4 + p*p*p / 27; 
        if( h > 0 ) return false; // only one real root

        e /= -3;

        if( h || q ){
                real d     = sqrt( q2 / 4 - h );
                real rc    = pow( -d, 1. / 3 ) * (d<0 ? -1 : 1);
                real theta = acos( -q / ( 2*d ) ) / 3;
                real costh = cos(theta);
                real sinth = SQRT_3 * sin(theta);
                       t     = e + rc * 2 * costh;
                       u     = e - rc * (costh + sinth);
                       v     = e - rc * (costh - sinth);
        } else t = u = v = e;
        return true;
}

uint gcf(uint a, uint b)
{
        while( b ){
                uint i = a;
                a      = b;
                b      = i%b;
        }
        return a;
}

uint lcm(uint a, uint b) {return a * (b/gcf(a, b));}

real sqr_heron(real a, real b, real c) {real p = (a + b + c) / 2; return p*(p-a)*(p-b)*(p-c);}
real     heron(real a, real b, real c) {return sqrt(sqr_heron(a, b, c));} 
real invsqrt  (real v)                 {assert(v > 0); return 1/sqrt(v);} 
real ACos     (real v)                 {return -1 < v ? (v < 1 ? acos(v) : 0) : PI;} 
real ASin     (real v)                 {return -1 < v ? (v < 1 ? asin(v) : HALF_PI) : -HALF_PI;} 
real ATan     (real v)                 {return atan(v);} 
real ATan2    (real y, real x)         {return x || y ? atan2(y, x) : 0;}// Mathematically, ATan2(0,0) is undefined, but ANSI standards require the function to return 0

real Log2(real v)
{
        if( v > 0 ) return INV_LN_2 * log(v);
        else {
                fail;
                return 0;
        }
}

real Log10(real v)
{
        if( v > 0 ) return INV_LN_10 * log(v);
        else {
                fail;
                return 0;
        }
}


real UnitRandom(uint seed)
{
	    if( seed > 0 ) srand(seed);
	    return rand()/((real)(RAND_MAX));
}

real SymmetricRandom(uint seed)                     {return 2*UnitRandom(seed) - 1;}
real IntervalRandom (real min, real max, uint seed) {return min + (max - min)*UnitRandom(seed);}

Vec3 spherical_to_cartesian(real rho, real phi, real theta) {real sin_phi = sin(phi); return rho * Vec3(sin_phi*cos(theta), cos(phi), sin_phi*sin(theta));}

void cartesian_to_spherical(real& rho, real& phi, real& theta, Vec3CR v)
{
        rho    = v.length_sqr();
        phi    = acos(v.z / rho);
        real S = asin(v.y / (v.x*v.x + v.y*v.y));
        theta  = v.x < 0 ? PI - S : S;
}