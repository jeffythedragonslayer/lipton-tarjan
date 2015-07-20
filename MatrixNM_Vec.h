#pragma once
#include "style.h"
struct Matrix22;
struct Vec2;

Matrix22 create_matrix22_from_rows     (Vec2CR, Vec2CR);
Matrix22 create_matrix22_from_cols     (Vec2CR, Vec2CR);
Vec2     get_row_vec2                  (Matrix22CR, uint);
Vec2     get_col_vec2                  (Matrix22CR, uint); 
Matrix23 create_matrix23_from_rows     (Vec2CR, Vec2CR, Vec2CR);
Vec2     get_row_vec2                  (Matrix23CR, uint);
 Matrix23 create_matrix23_from_cols    (Vec3CR, Vec3CR, Vec3CR);
Vec3     get_col_vec3                  (Matrix23CR, uint); 
Vec3     operator*                     (Vec3CR, Matrix33CR);
Vec3&    operator*=                    (Vec3&,    Matrix33CR); 
Matrix33 direct_product                (Vec3CR, Vec3CR);
Matrix33 rotation_matrix               (Vec3CR axis, real angle);
Vec3     get_scaling                   (Matrix34CR);
Vec3     get_scaling                   (Matrix33CR);
Matrix33 scaling_matrix                (Vec3CR scaling_factors);
Matrix33 create_matrix33_from_rows     (Vec3CR, Vec3CR, Vec3CR);
Matrix33 create_matrix33_from_cols     (Vec3CR, Vec3CR, Vec3CR);
Matrix33 create_matrix33_skew_symmetric(Vec3CR);
Vec3     get_row_vec3                  (Matrix33CR, uint r);
Vec3     get_col_vec3                  (Matrix33CR, uint c);

// u = right, v = up, w = look, q = pos
Matrix34         change_of_frame_matrix (Vec3CR u, Vec3CR v, Vec3CR w, Pt3CR);
Matrix34 reverse_change_of_frame_matrix (Vec3CR u, Vec3CR v, Vec3CR w, Pt3CR); 
Matrix34 lookatLH                       (Pt3CR eye, Vec3CR at, Vec3CR up);
Matrix34 lookatRH                       (Pt3CR eye, Vec3CR at, Vec3CR up);
Pt3      get_translation                (Matrix34CR);
Matrix34 translation_matrix             (Vec3CR); 
Vec3     operator*                      (Vec3CR, Matrix34CR); 
Vec3&    operator*=                     (Vec3&,  Matrix34CR);
Vec3     transform_inverse              (Vec3CR, Matrix34CR);
Vec3     transform_inverse_direction    (Vec3CR, Matrix34CR);
Matrix34 create_matrix34_from_rows      (Vec3CR, Vec3CR, Vec3CR, Vec3CR);
Vec3     get_row_vec3                   (Matrix34CR, uint); 
Matrix34 create_matrix34_from_cols      (Vec4CR, Vec4CR, Vec4CR);
Vec4     get_col_vec4                   (Matrix34CR, uint); 
Vec3     operator*                      (Vec3CR, Matrix44CR);
Vec4     operator*                      (Vec4CR, Matrix44CR);
Vec3&    operator*=                     (Vec4&,  Matrix34CR);
Vec4&    operator*=                     (Vec4&,  Matrix44CR);
Matrix44 create_matrix44_from_rows      (Vec4CR, Vec4CR, Vec4CR, Vec4CR);
Matrix44 create_matrix44_from_cols      (Vec4CR, Vec4CR, Vec4CR, Vec4CR);
Vec4     get_row_vec4                   (Matrix44CR, uint);
Vec4     get_col_vec4                   (Matrix44CR, uint);