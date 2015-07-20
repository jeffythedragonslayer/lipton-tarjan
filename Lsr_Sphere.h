#pragma once
struct Sphere;
struct Seg3;
struct Ray3;
#include "Vec.h"

bool segcast_sphere              (Seg3CR, SphereCR);
bool segcast_sphere              (real& t, Pt3& q, Seg3CR, SphereCR);
bool sweep_unitsphere_against_pt (real& t, Ray3CR, Pt3CR p);
bool sweep_unitsphere_against_seg(real& t, Pt3& q, Ray3CR, Seg3CR);
bool  raycast_sphere             (Ray3CR, SphereCR);
bool  raycast_sphere             (real& t, Pt3& q, Ray3CR, SphereCR);
bool linecast_sphere             (Ray3CR, SphereCR);
bool linecast_sphere             (real& t, Pt3& q, Ray3CR, SphereCR);