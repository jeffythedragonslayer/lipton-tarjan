#pragma once
#include "mathutil.h"
struct Seg3;
struct Capsule;

bool segcast_capsule(Seg3CR, CapsuleCR);
bool segcast_capsule(real& t, Seg3CR, CapsuleCR);
