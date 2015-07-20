#pragma once
struct Seg3;
struct Tri3;
struct Ray3;
struct BaryPt;
#include "Vec.h"

bool  segcast_tri(BaryPt&, real& t, Seg3CR, Tri3CR);
bool  segcast_tri(         real& t, Seg3CR, Tri3CR);
bool  raycast_tri(BaryPt&, real& t, Ray3CR, Tri3CR);
bool  raycast_tri(         real& t, Ray3CR, Tri3CR);
bool linecast_tri(BaryPt&,          Ray3CR, Tri3CR);
bool linecast_tri(                  Ray3CR, Tri3CR);