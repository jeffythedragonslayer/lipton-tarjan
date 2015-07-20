#include "stdafx.h"
#include "mathdebug.h"
#include "Vec.h"
#include "Vec.h"
#include "Vec.h"
#include "Quat.h"
#include "Tri.h"
#include "Plane.h"
#include "Matrix.h"
using namespace std;

 ostream& operator<<( ostream& o, Vec2CR  v) {o << "("   << v.x << ", "  << v.y << ")";                               return o;}
 ostream& operator<<( ostream& o, Vec3CR  v) {o << "("   << v.x << ", "  << v.y << ", " << v.z << ")";                return o;}
 ostream& operator<<( ostream& o, Vec4CR  v) {o << "("   << v.x << ", "  << v.y << ", " << v.z << ", " << v.w << ")"; return o;}
 ostream& operator<<( ostream& o, Vec2uCR v) {o << "("   << v.x << ", "  << v.y << ")";                               return o;}
 ostream& operator<<( ostream& o, QuatCR  q) {o << "("   << q.v << ", "  << q.w << ")";                               return o;}
 ostream& operator<<( ostream& o, PlaneCR p) {o << "(n=" << p.n << " d=" << p.d << ")";                               return o;}
 ostream& operator<<( ostream& o, Tri2CR  t) {o << "(a=" << t.a << " b=" << t.b << " c=" << t.c << ")";               return o;}
 ostream& operator<<( ostream& o, Tri3CR  t) {o << "(a=" << t.a << " b=" << t.b << " c=" << t.c << ")";               return o;} 
wostream& operator<<(wostream& o, Vec2CR  v) {o << "("   << v.x << ", "  << v.y << ")";                               return o;}
wostream& operator<<(wostream& o, Vec3CR  v) {o << "("   << v.x << ", "  << v.y << ", " << v.z << ")";                return o;}
wostream& operator<<(wostream& o, Vec4CR  v) {o << "("   << v.x << ", "  << v.y << ", " << v.z << ", " << v.w << ")"; return o;}
wostream& operator<<(wostream& o, Vec2uCR v) {o << "("   << v.x << ", "  << v.y << ")";                               return o;}
wostream& operator<<(wostream& o, QuatCR  q) {o << "("   << q.v << ", "  << q.w << ")";                               return o;}
wostream& operator<<(wostream& o, PlaneCR p) {o << "(n=" << p.n << " d=" << p.d << ")";                               return o;}
wostream& operator<<(wostream& o, Tri2CR  t) {o << "(a=" << t.a << " b=" << t.b << " c=" << t.c << ")";               return o;}
wostream& operator<<(wostream& o, Tri3CR  t) {o << "(a=" << t.a << " b=" << t.b << " c=" << t.c << ")";               return o;}

std::ostream& operator<< (std::ostream& o, const Matrix<real>& that)
{
        for( uint r = 0; r < that.num_rows(); ++r ){
                o << "[";
                for( uint c = 0; c < that.num_cols(); ++c ) o << that(r, c) << ' ';
                o << "]\n";
        }
        return o;
}

std::wostream& operator<< (std::wostream& o, const Matrix<real>& that)
{
        for( uint r = 0; r < that.num_rows(); ++r ){
                o << "[";
                for( uint c = 0; c < that.num_cols(); ++c ) o << that(r, c) << ' ';
                o << "]\n";
        }
        return o;
}

std::wistream& operator>>(std::wistream& is, Vec3& v) {is >> v.x >> v.y >> v.z; return is;}
std::wistream& operator>>(std::wistream& is, Vec2& v) {is >> v.x >> v.y;        return is;}