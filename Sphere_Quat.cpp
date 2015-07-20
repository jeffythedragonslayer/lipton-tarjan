#include "stdafx.h"
#include "Sphere_Quat.h"
#include "Sphere.h"
#include "Quat.h"

Sphere transform_sphere(SphereCR s, real scale, QuatCR rot, Vec3CR W) {return Sphere(transform_vector(s.c * scale, rot) + W, s.r * scale);} // transform a sphere by an angle preserving transform