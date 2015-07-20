#pragma once
struct Sphere;
struct SSTPacket;
struct Tri3;
#include "Vec.h" 

bool      test                        (      SphereCR, Tri3CR);
bool      test                        (Pt3&, SphereCR, Tri3CR);
bool      pierce                      (Pt3&, SphereCR, Tri3CR);
bool      pierce                      (      SphereCR, Tri3CR);
SSTPacket sweep_unitsphere_against_tri(Ray3CR, Tri3CR);