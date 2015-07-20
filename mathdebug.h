#pragma once
#include "style.h"
#include "Matrix.h"
#include "mathutil.h"
#include <ostream>
struct Vec2;
struct Vec2u;
struct Vec3;
struct Vec4;
struct Quat;
struct Plane;
struct Tri2;
struct Tri3;
struct Matrix22;
struct Matrix33;
struct Matrix44;

std:: ostream& operator<<(std:: ostream&, Vec2CR);
std:: ostream& operator<<(std:: ostream&, Vec3CR);
std:: ostream& operator<<(std:: ostream&, Vec4CR);
std:: ostream& operator<<(std:: ostream&, Vec2uCR);
std:: ostream& operator<<(std:: ostream&, QuatCR);
std:: ostream& operator<<(std:: ostream&, PlaneCR);
std:: ostream& operator<<(std:: ostream&, Tri2CR);
std:: ostream& operator<<(std:: ostream&, Tri3CR);
std:: ostream& operator<<(std:: ostream&, Matrix22CR);
std:: ostream& operator<<(std:: ostream&, Matrix33CR);
std:: ostream& operator<<(std:: ostream&, Matrix44CR);
std:: ostream& operator<<(std:: ostream&, Matrix<real> const&);

std::wostream& operator<<(std::wostream&, Vec2CR);
std::wostream& operator<<(std::wostream&, Vec3CR);
std::wostream& operator<<(std::wostream&, Vec4CR);
std::wostream& operator<<(std::wostream&, Vec2uCR);
std::wostream& operator<<(std::wostream&, QuatCR);
std::wostream& operator<<(std::wostream&, PlaneCR);
std::wostream& operator<<(std::wostream&, Tri2CR);
std::wostream& operator<<(std::wostream&, Tri3CR);
std:: ostream& operator<<(std::wostream&, Matrix22CR);
std:: ostream& operator<<(std::wostream&, Matrix33CR);
std::wostream& operator<<(std::wostream&, Matrix44CR);
std::wostream& operator<<(std::wostream&, Matrix<real> const&);

std::wistream& operator>>(std::wistream& is, Vec3& v);
std::wistream& operator>>(std::wistream& is, Vec2& v);