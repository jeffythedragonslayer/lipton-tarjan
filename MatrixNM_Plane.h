#pragma once
#include "style.h"
struct Matrix34;
struct Plane;

Matrix34 reflection_matrix(PlaneCR);
Matrix44 shadow_matrix    (Vec4CR l, PlaneCR p);