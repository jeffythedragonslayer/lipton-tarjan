#pragma once
#include "style.h"
struct Matrix44;
struct Frustum6;

Frustum6 create_frustum6                        (Matrix44CR VP);
FrustumQ create_frustumQ_from_perspective_matrix(Matrix44CR P);