#pragma once
#define _USE_MATH_DEFINES
#include "style.h"
#include <boost/algorithm/clamp.hpp>
#include <limits>
#include <math.h>

#define REAL_FLOAT
#ifdef REAL_FLOAT
        //typedef float real;
        #define real float
#else
        //typedef double real;
        #define real double
#endif

typedef real*       realP;
typedef real const* realCP;
typedef real const& realCR;

#define INF std::numeric_limits<real>::infinity()
const real  EP = .0001;
const real  PI      = M_PI;
const real  TAU     = 2 * PI;
//const real  ep   = FLT_ep;
//const real  ZERO_tol  = 1e-06f;
//const real  MAX_REAL  = FLT_MAX;
const real  HALF_PI    = M_PI_2;
const real  QUARTER_PI = HALF_PI / 2;
const real  DEG_TO_RAD = PI  / 180;
const real  RAD_TO_DEG = 180 / PI;
const real  E          = M_E;
const real  LOG_2E     = M_LOG2E;
const real  LOG_10E    = M_LOG10E;
const real  LN_2       = M_LN2;
const real  LN_10      = M_LN10;
const real  SQRT_2     = M_SQRT2;
const real  SQRT_3     = sqrt(3);
const real  SQRT_5     = sqrt(5);
const real  PHI        = (1+SQRT_5)/2;
const real  PHI2       = PHI  * PHI;
const real  PHI3       = PHI2 * PHI;
const real  INV_PI     = M_1_PI;
const real  INV_TAU    = 1 / TAU;
const real  INV_SQRT_2 = M_SQRT1_2;
const real  INV_SQRT_3 = 1 / SQRT_3;
const real  INV_PHI    = 1 / PHI;
const real  INV_LN_2   = 1 / LN_2;
const real  INV_LN_10  = 1 / LN_10;

real abs_equals(real a, real b, real tol);
real rel_equals(real a, real b, real tol);
real wrapvalue (real);
real rwa       (real oldx, real newx, real bias);

uint ipow              (uint b, uint e);
uint factorial         (uint);
bool is_pow_of_2       (uint);
bool is_pow_of_2       (int);
uint log2_of_pow2      (uint);
int  log2_of_pow2      (int);
int  scale_FLOAT_to_int(float, int pow);

//real awayfromzero_round(real x);

real csc (real);
real sec (real);
real cot (real);

real rands();               // rts random real in [0, 1)
real rands(real a, real b); // rts random real in [a, b)

template<typename T> T minimize (T&, T);
template<typename T> T maximize (T&, T);
template<typename T> int sgn(T);
real saturate             (real);
real sqr                  (real);
real cube                 (real);
real invsqrt              (real);
real is_real              (real);

real lerp                           (real a, real b,          real t);
real catmullrom_interpolate(real x0, real a, real b, real x3, real t);
real    hermite_interpolate(real x0, real a, real b, real x3, real t, real tension, real bias);
real     cosine_interpolate         (real a, real b,          real t);
real      cubic_interpolate(real x0, real a, real b, real x3, real t);
real     bezier_interpolate(real x0, real a, real b, real x3, real t);

bool          quadratic_roots(real& root1, real& root2, real a, real b, real c);
bool   higher_quadratic_root (real& root,               real a, real b, real c);
bool    lower_quadratic_root (real& root,               real a, real b, real c);

bool   solve_cubic          (real& t, real& u, real& v, real e, real f, real g);

uint   gcf                  (uint, uint);
uint   lcm                  (uint, uint);

real sqr_heron             (real, real, real);
real     heron             (real, real, real);

// TODO.  Provide SIMD versions of the implementations for whatever platforms we support
// Wrappers to hide implementations of functions.  ACos and ASin functions clamp the input argument to [-1,1] to avoid NaN issues when the input is slightly larger than 1 or slightly smaller than -1.  Other functions have the potential for using a fast and approximate algorithm rather than calling the standard math library functions
real Log  (real);
real Log2 (real);
real Log10(real);

real UnitRandom     (uint seed = 0); // Generate a random number in [0,1].  random number generator may be seeded by a first call to UnitRandom with a positive seed
real SymmetricRandom(uint seed = 0); // Generate a random number in [-1,1].  random number generator may be seeded by a first call to SymmetricRandom with a positive seed
real IntervalRandom (real min, real max, uint seed = 0); // Generate a random number in [min,max].  random number generator may be seeded by a first call to IntervalRandom with a positive seed

Vec3 spherical_to_cartesian(real rho, real phi, real theta);
void cartesian_to_spherical(real& pho, real& phi, real& theta, Vec3CR);

#include "mathutil.inl"
