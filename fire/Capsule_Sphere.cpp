#include "stdafx.h"
#include "Capsule_Sphere.h"
#include "Capsule.h"
#include "Sphere.h"
using namespace std;

bool test(SphereCR s, CapsuleCR capsule) {return capsule.seg.sqr_dist_to(s.c) <= sqr(s.r + capsule.r);}