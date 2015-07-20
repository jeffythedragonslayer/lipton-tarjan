#pragma once
#include "Vec.h"
struct Ray3;
struct Plane;

bool raycast_plane (real& t, Pt3& q, Ray3CR, PlaneCR);
bool linecast_plane(real& t, Pt3& q, Ray3CR, PlaneCR);
bool segcast_plane (real& t, Pt3& q, Seg3CR, PlaneCR);