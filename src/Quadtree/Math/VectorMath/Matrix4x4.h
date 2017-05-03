/*******************************************************************************
* File:           Matrix4x4.h
* Author:         Christian Alfons
* Date created:   2009-07-17
* Date modified:  2009-11-26
* Description:    A simple four-by-four matrix class, intended for use with
*                 homogeneous coordinates.
*******************************************************************************/

#ifndef MATRIX4X4_H_INCLUDED
#define MATRIX4X4_H_INCLUDED

#include <algorithm>

// Represents a four-by-four matrix
template<typename Real>
class Matrix4x4
{
  public:
    // Instance variables
    union
    {
      Real data[4][4];
      Real data1d[16];

      struct
      {
        Real m00;
        Real m01;
        Real m02;
        Real m03;
        Real m10;
        Real m11;
        Real m12;
        Real m13;
        Real m20;
        Real m21;
        Real m22;
        Real m23;
        Real m30;
        Real m31;
        Real m32;
        Real m33;
      };
    };

    // Class methods
    static inline Matrix4x4 CreateZeroMatrix();
    static inline Matrix4x4 CreateIdentityMatrix();
    static inline Matrix4x4 CreateScalingMatrix(const Real scale);
    static inline Matrix4x4 CreateScalingMatrix(const Real scaleX, const Real scaleY, const Real scaleZ);
    static inline Matrix4x4 CreateTranslationMatrix(const Real deltaX, const Real deltaY, const Real deltaZ);
    static inline Matrix4x4 CreateRotationMatrixX(const Real radians);
    static inline Matrix4x4 CreateRotationMatrixY(const Real radians);
    static inline Matrix4x4 CreateRotationMatrixZ(const Real radians);

    // Constructors
    inline Matrix4x4();
    inline Matrix4x4(const Real n00, const Real n01, const Real n02, const Real n03,
                     const Real n10, const Real n11, const Real n12, const Real n13,
                     const Real n20, const Real n21, const Real n22, const Real n23,
                     const Real n30, const Real n31, const Real n32, const Real n33);
    inline Matrix4x4(const Real val);
    inline Matrix4x4(const Real* vals);
    inline Matrix4x4(const Matrix3x3<Real> &mat3, const Vector3<Real> &vec3 = Vector3<Real>(0.0, 0.0, 0.0));

    // Operators
    inline Real          operator[](const unsigned int index) const;
    inline Real&         operator[](const unsigned int index);
    inline bool          operator==(const Matrix4x4& mat4) const;
    inline bool          operator!=(const Matrix4x4& mat4) const;
    inline Matrix4x4     operator+(const Matrix4x4& mat4) const;
    inline Matrix4x4     operator-(const Matrix4x4& mat4) const;
    inline Matrix4x4     operator*(const Matrix4x4& mat4) const;
    inline Matrix4x4     operator/(const Matrix4x4& mat4) const;
    inline Matrix4x4     operator+(const Real val) const;
    inline Matrix4x4     operator-(const Real val) const;
    inline Matrix4x4     operator*(const Real val) const;
    inline Matrix4x4     operator/(const Real val) const;
    inline Matrix4x4&    operator+=(const Matrix4x4& mat4);
    inline Matrix4x4&    operator-=(const Matrix4x4& mat4);
    inline Matrix4x4&    operator*=(const Matrix4x4& mat4);
    inline Matrix4x4&    operator/=(const Matrix4x4& mat4);
    inline Matrix4x4&    operator+=(const Real val);
    inline Matrix4x4&    operator-=(const Real val);
    inline Matrix4x4&    operator*=(const Real val);
    inline Matrix4x4&    operator/=(const Real val);
    inline Vector3<Real> operator*(const Vector3<Real>& vec3) const;
    inline Vector4<Real> operator*(const Vector4<Real>& vec4) const;

    // Instance methods
    inline Real           GetElement(const unsigned int row, const unsigned int col) const;
    inline Real&          GetElement(const unsigned int row, const unsigned int col);
    inline Vector4<Real>  GetRow(const unsigned int row) const;
    inline Vector4<Real>  GetColumn(const unsigned int col) const;
    inline Matrix4x4      MultiplyElementWise(const Matrix4x4& matrix);
    inline Matrix4x4      GetTranspose() const;
    inline Matrix4x4      GetInverse() const;
    inline Matrix4x4&     Transpose();
    inline Matrix4x4&     Invert();
};

template<typename Real>
Matrix4x4<Real> Matrix4x4<Real>::CreateZeroMatrix()
{
  return Matrix4x4(Real(0.0));
}

template<typename Real>
Matrix4x4<Real> Matrix4x4<Real>::CreateIdentityMatrix()
{
  Matrix4x4 identityMatrix(Real(0.0));

  identityMatrix.m00 = identityMatrix.m11 = identityMatrix.m22 = identityMatrix.m33 = Real(1.0);

  return identityMatrix;
}

template<typename Real>
Matrix4x4<Real> Matrix4x4<Real>::CreateScalingMatrix(const Real scale)
{
  Matrix4x4 scalingMatrix;

  scalingMatrix.m00 = scalingMatrix.m11 = scalingMatrix.m22 = scale;

  return scalingMatrix;
}

template<typename Real>
Matrix4x4<Real> Matrix4x4<Real>::CreateScalingMatrix(const Real scaleX, const Real scaleY, const Real scaleZ)
{
  Matrix4x4 scalingMatrix;

  scalingMatrix.m00 = scaleX;
  scalingMatrix.m11 = scaleY;
  scalingMatrix.m22 = scaleZ;

  return scalingMatrix;
}

template<typename Real>
Matrix4x4<Real> Matrix4x4<Real>::CreateTranslationMatrix(const Real deltaX, const Real deltaY, const Real deltaZ)
{
  Matrix4x4 translationMatrix;

  translationMatrix.m03 = deltaX;
  translationMatrix.m13 = deltaY;
  translationMatrix.m23 = deltaZ;

  return translationMatrix;
}

template<typename Real>
Matrix4x4<Real> Matrix4x4<Real>::CreateRotationMatrixX(const Real radians)
{
  Matrix4x4 rotationMatrixX;

  rotationMatrixX.m11 =  cos(radians);
  rotationMatrixX.m12 = -sin(radians);
  rotationMatrixX.m21 =  sin(radians);
  rotationMatrixX.m22 =  cos(radians);

  return rotationMatrixX;
}

template<typename Real>
Matrix4x4<Real> Matrix4x4<Real>::CreateRotationMatrixY(const Real radians)
{
  Matrix4x4 rotationMatrixY;

  rotationMatrixY.m00 =  cos(radians);
  rotationMatrixY.m02 =  sin(radians);
  rotationMatrixY.m20 = -sin(radians);
  rotationMatrixY.m22 =  cos(radians);

  return rotationMatrixY;
}

template<typename Real>
Matrix4x4<Real> Matrix4x4<Real>::CreateRotationMatrixZ(const Real radians)
{
  Matrix4x4 rotationMatrixZ;

  rotationMatrixZ.m00 =  cos(radians);
  rotationMatrixZ.m01 = -sin(radians);
  rotationMatrixZ.m10 =  sin(radians);
  rotationMatrixZ.m11 =  cos(radians);

  return rotationMatrixZ;
}

template<typename Real>
Matrix4x4<Real>::Matrix4x4()
{
  *this = CreateIdentityMatrix();
}

template<typename Real>
Matrix4x4<Real>::Matrix4x4(const Real n00, const Real n01, const Real n02, const Real n03,
                           const Real n10, const Real n11, const Real n12, const Real n13,
                           const Real n20, const Real n21, const Real n22, const Real n23,
                           const Real n30, const Real n31, const Real n32, const Real n33)
{
  m00 = n00;
  m01 = n01;
  m02 = n02;
  m03 = n03;
  m10 = n10;
  m11 = n11;
  m12 = n12;
  m13 = n13;
  m20 = n20;
  m21 = n21;
  m22 = n22;
  m23 = n23;
  m30 = n30;
  m31 = n31;
  m32 = n32;
  m33 = n33;
}

template<typename Real>
Matrix4x4<Real>::Matrix4x4(const Real val)
{
  m00 = m01 = m02 = m03 = m10 = m11 = m12 = m13 = m20 = m21 = m22 = m23 = m30 = m31 = m32 = m33 = val;
}

template<typename Real>
Matrix4x4<Real>::Matrix4x4(const Real* vals)
{
  data1d[0]  = vals[0];
  data1d[1]  = vals[1];
  data1d[2]  = vals[2];
  data1d[3]  = vals[3];
  data1d[4]  = vals[4];
  data1d[5]  = vals[5];
  data1d[6]  = vals[6];
  data1d[7]  = vals[7];
  data1d[8]  = vals[8];
  data1d[9]  = vals[9];
  data1d[10] = vals[10];
  data1d[11] = vals[11];
  data1d[12] = vals[12];
  data1d[13] = vals[13];
  data1d[14] = vals[14];
  data1d[15] = vals[15];
}

template<typename Real>
Matrix4x4<Real>::Matrix4x4(const Matrix3x3<Real> &mat3, const Vector3<Real> &vec3)
{
  m00 = mat3.m00;
  m01 = mat3.m01;
  m02 = mat3.m02;
  m10 = mat3.m10;
  m11 = mat3.m11;
  m12 = mat3.m12;
  m20 = mat3.m20;
  m21 = mat3.m21;
  m22 = mat3.m22;
  m03 = vec3.x;
  m13 = vec3.y;
  m23 = vec3.z;
  m30 = Real(0.0);
  m31 = Real(0.0);
  m32 = Real(0.0);
  m33 = Real(1.0);
}

template<typename Real>
Real Matrix4x4<Real>::operator[](const unsigned int index) const
{
  return data1d[index];
}

template<typename Real>
Real& Matrix4x4<Real>::operator[](const unsigned int index)
{
  return data1d[index];
}

template<typename Real>
bool Matrix4x4<Real>::operator==(const Matrix4x4& mat4) const
{
  return m00 == mat4.m00 && m01 == mat4.m01 && m02 == mat4.m02 && m03 == mat4.m03
      && m10 == mat4.m10 && m11 == mat4.m11 && m12 == mat4.m12 && m13 == mat4.m13
      && m20 == mat4.m20 && m21 == mat4.m21 && m22 == mat4.m22 && m23 == mat4.m23
      && m30 == mat4.m30 && m31 == mat4.m31 && m32 == mat4.m32 && m33 == mat4.m33;
}

template<typename Real>
bool Matrix4x4<Real>::operator!=(const Matrix4x4& mat4) const
{
  return !(*this == mat4);
}

template<typename Real>
Matrix4x4<Real> Matrix4x4<Real>::operator+(const Matrix4x4& mat4) const
{
  Matrix4x4 resultMatrix = *this;

  resultMatrix.m00 += mat4.m00;
  resultMatrix.m01 += mat4.m01;
  resultMatrix.m02 += mat4.m02;
  resultMatrix.m03 += mat4.m03;
  resultMatrix.m10 += mat4.m10;
  resultMatrix.m11 += mat4.m11;
  resultMatrix.m12 += mat4.m12;
  resultMatrix.m13 += mat4.m13;
  resultMatrix.m20 += mat4.m20;
  resultMatrix.m21 += mat4.m21;
  resultMatrix.m22 += mat4.m22;
  resultMatrix.m23 += mat4.m23;
  resultMatrix.m30 += mat4.m30;
  resultMatrix.m31 += mat4.m31;
  resultMatrix.m32 += mat4.m32;
  resultMatrix.m33 += mat4.m33;

  return resultMatrix;
}

template<typename Real>
Matrix4x4<Real> Matrix4x4<Real>::operator-(const Matrix4x4& mat4) const
{
  Matrix4x4 resultMatrix = *this;

  resultMatrix.m00 -= mat4.m00;
  resultMatrix.m01 -= mat4.m01;
  resultMatrix.m02 -= mat4.m02;
  resultMatrix.m03 -= mat4.m03;
  resultMatrix.m10 -= mat4.m10;
  resultMatrix.m11 -= mat4.m11;
  resultMatrix.m12 -= mat4.m12;
  resultMatrix.m13 -= mat4.m13;
  resultMatrix.m20 -= mat4.m20;
  resultMatrix.m21 -= mat4.m21;
  resultMatrix.m22 -= mat4.m22;
  resultMatrix.m23 -= mat4.m23;
  resultMatrix.m30 -= mat4.m30;
  resultMatrix.m31 -= mat4.m31;
  resultMatrix.m32 -= mat4.m32;
  resultMatrix.m33 -= mat4.m33;

  return resultMatrix;
}

template<typename Real>
Matrix4x4<Real> Matrix4x4<Real>::operator*(const Matrix4x4& mat4) const
{
  Matrix4x4 resultMatrix;

  resultMatrix.m00 = m00 * mat4.m00 + m01 * mat4.m10 + m02 * mat4.m20 + m03 * mat4.m30;
  resultMatrix.m01 = m00 * mat4.m01 + m01 * mat4.m11 + m02 * mat4.m21 + m03 * mat4.m31;
  resultMatrix.m02 = m00 * mat4.m02 + m01 * mat4.m12 + m02 * mat4.m22 + m03 * mat4.m32;
  resultMatrix.m03 = m00 * mat4.m03 + m01 * mat4.m13 + m02 * mat4.m23 + m03 * mat4.m33;
  resultMatrix.m10 = m10 * mat4.m00 + m11 * mat4.m10 + m12 * mat4.m20 + m13 * mat4.m30;
  resultMatrix.m11 = m10 * mat4.m01 + m11 * mat4.m11 + m12 * mat4.m21 + m13 * mat4.m31;
  resultMatrix.m12 = m10 * mat4.m02 + m11 * mat4.m12 + m12 * mat4.m22 + m13 * mat4.m32;
  resultMatrix.m13 = m10 * mat4.m03 + m11 * mat4.m13 + m12 * mat4.m23 + m13 * mat4.m33;
  resultMatrix.m20 = m20 * mat4.m00 + m21 * mat4.m10 + m22 * mat4.m20 + m23 * mat4.m30;
  resultMatrix.m21 = m20 * mat4.m01 + m21 * mat4.m11 + m22 * mat4.m21 + m23 * mat4.m31;
  resultMatrix.m22 = m20 * mat4.m02 + m21 * mat4.m12 + m22 * mat4.m22 + m23 * mat4.m32;
  resultMatrix.m23 = m20 * mat4.m03 + m21 * mat4.m13 + m22 * mat4.m23 + m23 * mat4.m33;
  resultMatrix.m30 = m30 * mat4.m00 + m31 * mat4.m10 + m32 * mat4.m20 + m33 * mat4.m30;
  resultMatrix.m31 = m30 * mat4.m01 + m31 * mat4.m11 + m32 * mat4.m21 + m33 * mat4.m31;
  resultMatrix.m32 = m30 * mat4.m02 + m31 * mat4.m12 + m32 * mat4.m22 + m33 * mat4.m32;
  resultMatrix.m33 = m30 * mat4.m03 + m31 * mat4.m13 + m32 * mat4.m23 + m33 * mat4.m33;

  return resultMatrix;
}

template<typename Real>
Matrix4x4<Real> Matrix4x4<Real>::operator/(const Matrix4x4& mat4) const
{
  Matrix4x4 resultMatrix = *this;

  resultMatrix.m00 /= mat4.m00;
  resultMatrix.m01 /= mat4.m01;
  resultMatrix.m02 /= mat4.m02;
  resultMatrix.m03 /= mat4.m03;
  resultMatrix.m10 /= mat4.m10;
  resultMatrix.m11 /= mat4.m11;
  resultMatrix.m12 /= mat4.m12;
  resultMatrix.m13 /= mat4.m13;
  resultMatrix.m20 /= mat4.m20;
  resultMatrix.m21 /= mat4.m21;
  resultMatrix.m22 /= mat4.m22;
  resultMatrix.m23 /= mat4.m23;
  resultMatrix.m30 /= mat4.m30;
  resultMatrix.m31 /= mat4.m31;
  resultMatrix.m32 /= mat4.m32;
  resultMatrix.m33 /= mat4.m33;

  return resultMatrix;
}

template<typename Real>
Matrix4x4<Real> Matrix4x4<Real>::operator+(const Real val) const
{
  Matrix4x4 resultMatrix = *this;

  resultMatrix.m00 += val;
  resultMatrix.m01 += val;
  resultMatrix.m02 += val;
  resultMatrix.m03 += val;
  resultMatrix.m10 += val;
  resultMatrix.m11 += val;
  resultMatrix.m12 += val;
  resultMatrix.m13 += val;
  resultMatrix.m20 += val;
  resultMatrix.m21 += val;
  resultMatrix.m22 += val;
  resultMatrix.m23 += val;
  resultMatrix.m30 += val;
  resultMatrix.m31 += val;
  resultMatrix.m32 += val;
  resultMatrix.m33 += val;

  return resultMatrix;
}

template<typename Real>
Matrix4x4<Real> Matrix4x4<Real>::operator-(const Real val) const
{
  Matrix4x4 resultMatrix = *this;

  resultMatrix.m00 -= val;
  resultMatrix.m01 -= val;
  resultMatrix.m02 -= val;
  resultMatrix.m03 -= val;
  resultMatrix.m10 -= val;
  resultMatrix.m11 -= val;
  resultMatrix.m12 -= val;
  resultMatrix.m13 -= val;
  resultMatrix.m20 -= val;
  resultMatrix.m21 -= val;
  resultMatrix.m22 -= val;
  resultMatrix.m23 -= val;
  resultMatrix.m30 -= val;
  resultMatrix.m31 -= val;
  resultMatrix.m32 -= val;
  resultMatrix.m33 -= val;

  return resultMatrix;
}

template<typename Real>
Matrix4x4<Real> Matrix4x4<Real>::operator*(const Real val) const
{
  Matrix4x4 resultMatrix = *this;

  resultMatrix.m00 *= val;
  resultMatrix.m01 *= val;
  resultMatrix.m02 *= val;
  resultMatrix.m03 *= val;
  resultMatrix.m10 *= val;
  resultMatrix.m11 *= val;
  resultMatrix.m12 *= val;
  resultMatrix.m13 *= val;
  resultMatrix.m20 *= val;
  resultMatrix.m21 *= val;
  resultMatrix.m22 *= val;
  resultMatrix.m23 *= val;
  resultMatrix.m30 *= val;
  resultMatrix.m31 *= val;
  resultMatrix.m32 *= val;
  resultMatrix.m33 *= val;

  return resultMatrix;
}

template<typename Real>
Matrix4x4<Real> Matrix4x4<Real>::operator/(const Real val) const
{
  Matrix4x4 resultMatrix = *this;

  resultMatrix.m00 /= val;
  resultMatrix.m01 /= val;
  resultMatrix.m02 /= val;
  resultMatrix.m03 /= val;
  resultMatrix.m10 /= val;
  resultMatrix.m11 /= val;
  resultMatrix.m12 /= val;
  resultMatrix.m13 /= val;
  resultMatrix.m20 /= val;
  resultMatrix.m21 /= val;
  resultMatrix.m22 /= val;
  resultMatrix.m23 /= val;
  resultMatrix.m30 /= val;
  resultMatrix.m31 /= val;
  resultMatrix.m32 /= val;
  resultMatrix.m33 /= val;

  return resultMatrix;
}

template<typename Real>
Matrix4x4<Real>& Matrix4x4<Real>::operator+=(const Matrix4x4& mat4)
{
  m00 += mat4.m00;
  m01 += mat4.m01;
  m02 += mat4.m02;
  m03 += mat4.m03;
  m10 += mat4.m10;
  m11 += mat4.m11;
  m12 += mat4.m12;
  m13 += mat4.m13;
  m20 += mat4.m20;
  m21 += mat4.m21;
  m22 += mat4.m22;
  m23 += mat4.m23;
  m30 += mat4.m30;
  m31 += mat4.m31;
  m32 += mat4.m32;
  m33 += mat4.m33;

  return *this;
}

template<typename Real>
Matrix4x4<Real>& Matrix4x4<Real>::operator-=(const Matrix4x4& mat4)
{
  m00 -= mat4.m00;
  m01 -= mat4.m01;
  m02 -= mat4.m02;
  m03 -= mat4.m03;
  m10 -= mat4.m10;
  m11 -= mat4.m11;
  m12 -= mat4.m12;
  m13 -= mat4.m13;
  m20 -= mat4.m20;
  m21 -= mat4.m21;
  m22 -= mat4.m22;
  m23 -= mat4.m23;
  m30 -= mat4.m30;
  m31 -= mat4.m31;
  m32 -= mat4.m32;
  m33 -= mat4.m33;

  return *this;
}

template<typename Real>
Matrix4x4<Real>& Matrix4x4<Real>::operator*=(const Matrix4x4& mat4)
{
  *this = *this * mat4;

  return *this;
}

template<typename Real>
Matrix4x4<Real>& Matrix4x4<Real>::operator/=(const Matrix4x4& mat4)
{
  m00 /= mat4.m00;
  m01 /= mat4.m01;
  m02 /= mat4.m02;
  m03 /= mat4.m03;
  m10 /= mat4.m10;
  m11 /= mat4.m11;
  m12 /= mat4.m12;
  m13 /= mat4.m13;
  m20 /= mat4.m20;
  m21 /= mat4.m21;
  m22 /= mat4.m22;
  m23 /= mat4.m23;
  m30 /= mat4.m30;
  m31 /= mat4.m31;
  m32 /= mat4.m32;
  m33 /= mat4.m33;

  return *this;
}

template<typename Real>
Matrix4x4<Real>& Matrix4x4<Real>::operator+=(const Real val)
{
  m00 += val;
  m01 += val;
  m02 += val;
  m03 += val;
  m10 += val;
  m11 += val;
  m12 += val;
  m13 += val;
  m20 += val;
  m21 += val;
  m22 += val;
  m23 += val;
  m30 += val;
  m31 += val;
  m32 += val;
  m33 += val;

  return *this;
}

template<typename Real>
Matrix4x4<Real>& Matrix4x4<Real>::operator-=(const Real val)
{
  m00 -= val;
  m01 -= val;
  m02 -= val;
  m03 -= val;
  m10 -= val;
  m11 -= val;
  m12 -= val;
  m13 -= val;
  m20 -= val;
  m21 -= val;
  m22 -= val;
  m23 -= val;
  m30 -= val;
  m31 -= val;
  m32 -= val;
  m33 -= val;

  return *this;
}

template<typename Real>
Matrix4x4<Real>& Matrix4x4<Real>::operator*=(const Real val)
{
  m00 *= val;
  m01 *= val;
  m02 *= val;
  m03 *= val;
  m10 *= val;
  m11 *= val;
  m12 *= val;
  m13 *= val;
  m20 *= val;
  m21 *= val;
  m22 *= val;
  m23 *= val;
  m30 *= val;
  m31 *= val;
  m32 *= val;
  m33 *= val;

  return *this;
}

template<typename Real>
Matrix4x4<Real>& Matrix4x4<Real>::operator/=(const Real val)
{
  m00 /= val;
  m01 /= val;
  m02 /= val;
  m03 /= val;
  m10 /= val;
  m11 /= val;
  m12 /= val;
  m13 /= val;
  m20 /= val;
  m21 /= val;
  m22 /= val;
  m23 /= val;
  m30 /= val;
  m31 /= val;
  m32 /= val;
  m33 /= val;

  return *this;
}

template<typename Real>
Vector3<Real> Matrix4x4<Real>::operator*(const Vector3<Real>& vec3) const
{
  Vector3<Real> resultVector;

  resultVector.x = vec3.x * m00 + vec3.y * m01 + vec3.z * m02;
  resultVector.y = vec3.x * m10 + vec3.y * m11 + vec3.z * m12;
  resultVector.z = vec3.x * m20 + vec3.y * m21 + vec3.z * m22;

  return resultVector;
}

template<typename Real>
Vector4<Real> Matrix4x4<Real>::operator*(const Vector4<Real>& vec4) const
{
  Vector4<Real> resultVector;

  resultVector.x = vec4.x * m00 + vec4.y * m01 + vec4.z * m02 + vec4.w * m03;
  resultVector.y = vec4.x * m10 + vec4.y * m11 + vec4.z * m12 + vec4.w * m13;
  resultVector.z = vec4.x * m20 + vec4.y * m21 + vec4.z * m22 + vec4.w * m23;
  resultVector.w = vec4.x * m30 + vec4.y * m31 + vec4.z * m32 + vec4.w * m33;

  return resultVector;
}

template<typename Real>
Real Matrix4x4<Real>::GetElement(const unsigned int row, const unsigned int col) const
{
  return data[row][col];
}

template<typename Real>
Real& Matrix4x4<Real>::GetElement(const unsigned int row, const unsigned int col)
{
  return data[row][col];
}

template<typename Real>
Vector4<Real> Matrix4x4<Real>::GetRow(const unsigned int row) const
{
  return Vector4<Real>(data[row][0], data[row][1], data[row][2], data[row][3]);
}

template<typename Real>
Vector4<Real> Matrix4x4<Real>::GetColumn(const unsigned int col) const
{
  return Vector4<Real>(data[0][col], data[1][col], data[2][col], data[3][col]);
}

template<typename Real>
Matrix4x4<Real> Matrix4x4<Real>::MultiplyElementWise(const Matrix4x4& mat4)
{
  Matrix4x4 resultMatrix = *this;

  resultMatrix.m00 *= mat4.m00;
  resultMatrix.m01 *= mat4.m01;
  resultMatrix.m02 *= mat4.m02;
  resultMatrix.m03 *= mat4.m03;
  resultMatrix.m10 *= mat4.m10;
  resultMatrix.m11 *= mat4.m11;
  resultMatrix.m12 *= mat4.m12;
  resultMatrix.m13 *= mat4.m13;
  resultMatrix.m20 *= mat4.m20;
  resultMatrix.m21 *= mat4.m21;
  resultMatrix.m22 *= mat4.m22;
  resultMatrix.m23 *= mat4.m23;
  resultMatrix.m30 *= mat4.m30;
  resultMatrix.m31 *= mat4.m31;
  resultMatrix.m32 *= mat4.m32;
  resultMatrix.m33 *= mat4.m33;

  return resultMatrix;
}

template<typename Real>
Matrix4x4<Real> Matrix4x4<Real>::GetTranspose() const
{
  return Matrix4x4(m00, m10, m20, m30, m01, m11, m21, m31, m02, m12, m22, m32, m03, m13, m23, m33);
}

template<typename Real>
Matrix4x4<Real> Matrix4x4<Real>::GetInverse() const
{
  // From "Numerical Recipes in C++: The Art of Scientific Computing"
  Matrix4x4<Real> a = Matrix4x4<Real>(*this);
  Matrix4x4<Real> b = CreateIdentityMatrix();
  int i, icol = 0, irow = 0, j, k, l, ll;
  Real big, dum, pivinv;
  int n = 4;
  int m = 4;

  Vector4<int> indxc, indxr, ipiv;

  for (j = 0; j < n; j++)
  {
    ipiv[j] = 0;
  }

  for (i=0; i < n; i++)
  {
    big = 0.0;

    for (j = 0; j < n; j++)
    {
      if (ipiv[j] != 1)
      {
        for (k = 0; k < n; k++)
        {
          if (ipiv[k] == 0)
          {
            if (fabs(a.data[j][k]) >= big)
            {
              big = fabs(a.data[j][k]);
              irow = j;
              icol = k;
            }
          }
        }
      }
    }

    ++(ipiv[icol]);

    if (irow != icol)
    {
      for (l = 0; l < n; l++)
      {
        std::swap(a.data[irow][l], a.data[icol][l]);
      }

      for (l = 0; l < m; l++)
      {
        std::swap(b.data[irow][l], b.data[icol][l]);
      }
    }

    indxr[i] = irow;
    indxc[i] = icol;

    pivinv = 1.0 / a.data[icol][icol];

    a.data[icol][icol] = 1.0;

    for (l = 0; l < n; l++)
    {
      a.data[icol][l] *= pivinv;
    }

    for (l = 0; l < m; l++)
    {
      b.data[icol][l] *= pivinv;
    }

    for (ll = 0; ll < n; ll++)
    {
      if (ll != icol)
      {
        dum = a.data[ll][icol];
        a.data[ll][icol] = 0.0;

        for (l = 0; l < n; l++)
        {
          a.data[ll][l] -= a.data[icol][l] * dum;
        }

        for (l = 0; l < m; l++)
        {
          b.data[ll][l] -= b.data[icol][l] * dum;
        }
      }
    }
  }

  for (l = n - 1; l >= 0; l--)
  {
    if (indxr[l] != indxc[l])
    {
      for (k = 0; k < n; k++)
      {
        std::swap(a.data[k][indxr[l]], a.data[k][indxc[l]]);
      }
    }
  }

  return a;
}

template<typename Real>
Matrix4x4<Real>& Matrix4x4<Real>::Transpose()
{
  return *this = GetTranspose();
}

template<typename Real>
Matrix4x4<Real>& Matrix4x4<Real>::Invert()
{
  return *this = GetInverse();
}

#endif
