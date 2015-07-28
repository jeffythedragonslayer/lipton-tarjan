#pragma once
struct Ray3;
struct Cylinder;
#include "mathutil.h"

bool segcast_cylinder(real& t, Ray3CR, CylinderCR);
