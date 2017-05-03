/*******************************************************************************
* File:           VectorMath.h
* Author:         Christian Alfons
* Date created:   2009-10-17
* Date modified:  2009-10-17
* Description:    A simple vector/matrix library intended for use with OpenGL.
*******************************************************************************/

#ifndef VECTORMATH_H_INCLUDED
#define VECTORMATH_H_INCLUDED

#include "VectorMath/Vector2.h"
#include "VectorMath/Vector3.h"
#include "VectorMath/Vector4.h"
#include "VectorMath/Matrix2x2.h"
#include "VectorMath/Matrix3x3.h"
#include "VectorMath/Matrix4x4.h"

// Double typedefs
typedef Vector2<double>   Vector2d;
typedef Vector3<double>   Vector3d;
typedef Vector4<double>   Vector4d;
typedef Matrix2x2<double> Matrix2d;
typedef Matrix2x2<double> Matrix2x2d;
typedef Matrix3x3<double> Matrix3d;
typedef Matrix3x3<double> Matrix3x3d;
typedef Matrix4x4<double> Matrix4d;
typedef Matrix4x4<double> Matrix4x4d;

// Float typedefs
typedef Vector2<float>    Vector2f;
typedef Vector3<float>    Vector3f;
typedef Vector4<float>    Vector4f;
typedef Matrix2x2<float>  Matrix2f;
typedef Matrix2x2<float>  Matrix2x2f;
typedef Matrix3x3<float>  Matrix3f;
typedef Matrix3x3<float>  Matrix3x3f;
typedef Matrix4x4<float>  Matrix4f;
typedef Matrix4x4<float>  Matrix4x4f;

// Int typedefs
typedef Vector2<int>      Vector2i;
typedef Vector3<int>      Vector3i;
typedef Vector4<int>      Vector4i;
typedef Matrix2x2<int>    Matrix2i;
typedef Matrix2x2<int>    Matrix2x2i;
typedef Matrix3x3<int>    Matrix3i;
typedef Matrix3x3<int>    Matrix3x3i;
typedef Matrix4x4<int>    Matrix4i;
typedef Matrix4x4<int>    Matrix4x4i;

// Double typedefs (GLSL-style)
typedef Vector2<double>   vec2;
typedef Vector3<double>   vec3;
typedef Vector4<double>   vec4;
typedef Matrix2x2<double> mat2;
typedef Matrix2x2<double> mat2x2;
typedef Matrix3x3<double> mat3;
typedef Matrix3x3<double> mat3x3;
typedef Matrix4x4<double> mat4;
typedef Matrix4x4<double> mat4x4;

#endif
