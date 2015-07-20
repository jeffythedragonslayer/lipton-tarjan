#pragma once
struct Ray3;
struct Quad3;
#include "Vec.h"

bool linecast_quad(Pt3& r, Ray3CR, Quad3CR);