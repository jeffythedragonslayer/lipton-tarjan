#pragma once
#include "style.h"
struct Matrix33;
struct Quat;

Matrix33 create_matrix33(QuatCR);
Quat     create_quat    (Matrix33CR);
Matrix34 create_matrix34(QuatCR, Pt3CR);