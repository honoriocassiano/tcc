/*******************************************************************************
* File:           Matrix3x3.h
* Author:         Christian Alfons
* Date created:   2009-07-20
* Date modified:  2009-07-20
* Description:    A simple three-by-three matrix class.
*******************************************************************************/

#ifndef MATRIX3X3_H_INCLUDED
#define MATRIX3X3_H_INCLUDED

// Represents a three-by-three matrix
template<typename Real>
class Matrix3x3
{
  public:
    // Instance variables
    union
    {
      Real data[3][3];
      Real data1d[9];

      struct
      {
        Real m00;
        Real m01;
        Real m02;
        Real m10;
        Real m11;
        Real m12;
        Real m20;
        Real m21;
        Real m22;
      };
    };

    // Class methods
    static inline Matrix3x3 CreateZeroMatrix();
    static inline Matrix3x3 CreateIdentityMatrix();
    static inline Matrix3x3 CreateScalingMatrix(const Real scale);
    static inline Matrix3x3 CreateScalingMatrix(const Real scaleX, const Real scaleY, const Real scaleZ);
    static inline Matrix3x3 CreateRotationMatrixX(const Real radians);
    static inline Matrix3x3 CreateRotationMatrixY(const Real radians);
    static inline Matrix3x3 CreateRotationMatrixZ(const Real radians);

    // Constructors
    inline Matrix3x3();
    inline Matrix3x3(const Real n00, const Real n01, const Real n02,
                     const Real n10, const Real n11, const Real n12,
                     const Real n20, const Real n21, const Real n22);
    inline Matrix3x3(const Real val);
    inline Matrix3x3(const Real* vals);

    // Operators
    inline Real          operator[](const unsigned int index) const;
    inline Real&         operator[](const unsigned int index);
    inline bool          operator==(const Matrix3x3& mat3) const;
    inline bool          operator!=(const Matrix3x3& mat3) const;
    inline Matrix3x3     operator+(const Matrix3x3& mat3) const;
    inline Matrix3x3     operator-(const Matrix3x3& mat3) const;
    inline Matrix3x3     operator*(const Matrix3x3& mat3) const;
    inline Matrix3x3     operator/(const Matrix3x3& mat3) const;
    inline Matrix3x3     operator+(const Real val) const;
    inline Matrix3x3     operator-(const Real val) const;
    inline Matrix3x3     operator*(const Real val) const;
    inline Matrix3x3     operator/(const Real val) const;
    inline Matrix3x3&    operator+=(const Matrix3x3& mat3);
    inline Matrix3x3&    operator-=(const Matrix3x3& mat3);
    inline Matrix3x3&    operator*=(const Matrix3x3& mat3);
    inline Matrix3x3&    operator/=(const Matrix3x3& mat3);
    inline Matrix3x3&    operator+=(const Real val);
    inline Matrix3x3&    operator-=(const Real val);
    inline Matrix3x3&    operator*=(const Real val);
    inline Matrix3x3&    operator/=(const Real val);
    inline Vector3<Real> operator*(const Vector3<Real>& vec3) const;
    inline Real          GetElement(const unsigned int row, const unsigned int col) const;
    inline Real&         GetElement(const unsigned int row, const unsigned int col);
    inline Vector3<Real> GetRow(const unsigned int row) const;
    inline Vector3<Real> GetColumn(const unsigned int col) const;

    // Instance methods
    inline Matrix3x3  MultiplyElementWise(const Matrix3x3& matrix);
    inline Matrix3x3  GetTranspose() const;
    inline Matrix3x3& Transpose();
    inline Matrix3x3& Orthonormalize();
};

template<typename Real>
Matrix3x3<Real> Matrix3x3<Real>::CreateZeroMatrix()
{
  return Matrix3x3(Real(0.0));
}

template<typename Real>
Matrix3x3<Real> Matrix3x3<Real>::CreateIdentityMatrix()
{
  Matrix3x3 identityMatrix(Real(0.0));

  identityMatrix.m00 = identityMatrix.m11 = identityMatrix.m22 = Real(1.0);

  return identityMatrix;
}

template<typename Real>
Matrix3x3<Real> Matrix3x3<Real>::CreateScalingMatrix(const Real scale)
{
  Matrix3x3 scalingMatrix;

  scalingMatrix.m00 = scalingMatrix.m11 = scalingMatrix.m22 = scale;

  return scalingMatrix;
}

template<typename Real>
Matrix3x3<Real> Matrix3x3<Real>::CreateScalingMatrix(const Real scaleX, const Real scaleY, const Real scaleZ)
{
  Matrix3x3 scalingMatrix;

  scalingMatrix.m00 = scaleX;
  scalingMatrix.m11 = scaleY;
  scalingMatrix.m22 = scaleZ;

  return scalingMatrix;
}

template<typename Real>
Matrix3x3<Real> Matrix3x3<Real>::CreateRotationMatrixX(const Real radians)
{
  Matrix3x3 rotationMatrixX;

  rotationMatrixX.m11 =  cos(radians);
  rotationMatrixX.m12 = -sin(radians);
  rotationMatrixX.m21 =  sin(radians);
  rotationMatrixX.m22 =  cos(radians);

  return rotationMatrixX;
}

template<typename Real>
Matrix3x3<Real> Matrix3x3<Real>::CreateRotationMatrixY(const Real radians)
{
  Matrix3x3 rotationMatrixY;

  rotationMatrixY.m00 =  cos(radians);
  rotationMatrixY.m02 =  sin(radians);
  rotationMatrixY.m20 = -sin(radians);
  rotationMatrixY.m22 =  cos(radians);

  return rotationMatrixY;
}

template<typename Real>
Matrix3x3<Real> Matrix3x3<Real>::CreateRotationMatrixZ(const Real radians)
{
  Matrix3x3 rotationMatrixZ;

  rotationMatrixZ.m00 =  cos(radians);
  rotationMatrixZ.m01 = -sin(radians);
  rotationMatrixZ.m10 =  sin(radians);
  rotationMatrixZ.m11 =  cos(radians);

  return rotationMatrixZ;
}

template<typename Real>
Matrix3x3<Real>::Matrix3x3()
{
  *this = CreateIdentityMatrix();
}

template<typename Real>
Matrix3x3<Real>::Matrix3x3(const Real n00, const Real n01, const Real n02,
                           const Real n10, const Real n11, const Real n12,
                           const Real n20, const Real n21, const Real n22)
{
  m00 = n00;
  m01 = n01;
  m02 = n02;
  m10 = n10;
  m11 = n11;
  m12 = n12;
  m20 = n20;
  m21 = n21;
  m22 = n22;
}

template<typename Real>
Matrix3x3<Real>::Matrix3x3(const Real val)
{
  m00 = m01 = m02 = m10 = m11 = m12 = m20 = m21 = m22 = val;
}

template<typename Real>
Matrix3x3<Real>::Matrix3x3(const Real* vals)
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
}

template<typename Real>
Real Matrix3x3<Real>::operator[](const unsigned int index) const
{
  return data1d[index];
}

template<typename Real>
Real& Matrix3x3<Real>::operator[](const unsigned int index)
{
  return data1d[index];
}

template<typename Real>
bool Matrix3x3<Real>::operator==(const Matrix3x3& mat3) const
{
  return m00 == mat3.m00 && m01 == mat3.m01 && m02 == mat3.m02
      && m10 == mat3.m10 && m11 == mat3.m11 && m12 == mat3.m12
      && m20 == mat3.m20 && m21 == mat3.m21 && m22 == mat3.m22;
}

template<typename Real>
bool Matrix3x3<Real>::operator!=(const Matrix3x3& mat3) const
{
  return !(*this == mat3);
}

template<typename Real>
Matrix3x3<Real> Matrix3x3<Real>::operator+(const Matrix3x3& mat3) const
{
  Matrix3x3 resultMatrix = *this;

  resultMatrix.m00 += mat3.m00;
  resultMatrix.m01 += mat3.m01;
  resultMatrix.m02 += mat3.m02;
  resultMatrix.m10 += mat3.m10;
  resultMatrix.m11 += mat3.m11;
  resultMatrix.m12 += mat3.m12;
  resultMatrix.m20 += mat3.m20;
  resultMatrix.m21 += mat3.m21;
  resultMatrix.m22 += mat3.m22;

  return resultMatrix;
}

template<typename Real>
Matrix3x3<Real> Matrix3x3<Real>::operator-(const Matrix3x3& mat3) const
{
  Matrix3x3 resultMatrix = *this;

  resultMatrix.m00 -= mat3.m00;
  resultMatrix.m01 -= mat3.m01;
  resultMatrix.m02 -= mat3.m02;
  resultMatrix.m10 -= mat3.m10;
  resultMatrix.m11 -= mat3.m11;
  resultMatrix.m12 -= mat3.m12;
  resultMatrix.m20 -= mat3.m20;
  resultMatrix.m21 -= mat3.m21;
  resultMatrix.m22 -= mat3.m22;

  return resultMatrix;
}

template<typename Real>
Matrix3x3<Real> Matrix3x3<Real>::operator*(const Matrix3x3& mat3) const
{
  Matrix3x3 resultMatrix;

  resultMatrix.m00 = m00 * mat3.m00 + m01 * mat3.m10 + m02 * mat3.m20;
  resultMatrix.m01 = m00 * mat3.m01 + m01 * mat3.m11 + m02 * mat3.m21;
  resultMatrix.m02 = m00 * mat3.m02 + m01 * mat3.m12 + m02 * mat3.m22;
  resultMatrix.m10 = m10 * mat3.m00 + m11 * mat3.m10 + m12 * mat3.m20;
  resultMatrix.m11 = m10 * mat3.m01 + m11 * mat3.m11 + m12 * mat3.m21;
  resultMatrix.m12 = m10 * mat3.m02 + m11 * mat3.m12 + m12 * mat3.m22;
  resultMatrix.m20 = m20 * mat3.m00 + m21 * mat3.m10 + m22 * mat3.m20;
  resultMatrix.m21 = m20 * mat3.m01 + m21 * mat3.m11 + m22 * mat3.m21;
  resultMatrix.m22 = m20 * mat3.m02 + m21 * mat3.m12 + m22 * mat3.m22;

  return resultMatrix;
}

template<typename Real>
Matrix3x3<Real> Matrix3x3<Real>::operator/(const Matrix3x3& mat3) const
{
  Matrix3x3 resultMatrix = *this;

  resultMatrix.m00 /= mat3.m00;
  resultMatrix.m01 /= mat3.m01;
  resultMatrix.m02 /= mat3.m02;
  resultMatrix.m10 /= mat3.m10;
  resultMatrix.m11 /= mat3.m11;
  resultMatrix.m12 /= mat3.m12;
  resultMatrix.m20 /= mat3.m20;
  resultMatrix.m21 /= mat3.m21;
  resultMatrix.m22 /= mat3.m22;

  return resultMatrix;
}

template<typename Real>
Matrix3x3<Real> Matrix3x3<Real>::operator+(const Real val) const
{
  Matrix3x3 resultMatrix = *this;

  resultMatrix.m00 += val;
  resultMatrix.m01 += val;
  resultMatrix.m02 += val;
  resultMatrix.m10 += val;
  resultMatrix.m11 += val;
  resultMatrix.m12 += val;
  resultMatrix.m20 += val;
  resultMatrix.m21 += val;
  resultMatrix.m22 += val;

  return resultMatrix;
}

template<typename Real>
Matrix3x3<Real> Matrix3x3<Real>::operator-(const Real val) const
{
  Matrix3x3 resultMatrix = *this;

  resultMatrix.m00 -= val;
  resultMatrix.m01 -= val;
  resultMatrix.m02 -= val;
  resultMatrix.m10 -= val;
  resultMatrix.m11 -= val;
  resultMatrix.m12 -= val;
  resultMatrix.m20 -= val;
  resultMatrix.m21 -= val;
  resultMatrix.m22 -= val;

  return resultMatrix;
}

template<typename Real>
Matrix3x3<Real> Matrix3x3<Real>::operator*(const Real val) const
{
  Matrix3x3 resultMatrix = *this;

  resultMatrix.m00 *= val;
  resultMatrix.m01 *= val;
  resultMatrix.m02 *= val;
  resultMatrix.m10 *= val;
  resultMatrix.m11 *= val;
  resultMatrix.m12 *= val;
  resultMatrix.m20 *= val;
  resultMatrix.m21 *= val;
  resultMatrix.m22 *= val;

  return resultMatrix;
}

template<typename Real>
Matrix3x3<Real> Matrix3x3<Real>::operator/(const Real val) const
{
  Matrix3x3 resultMatrix = *this;

  resultMatrix.m00 /= val;
  resultMatrix.m01 /= val;
  resultMatrix.m02 /= val;
  resultMatrix.m10 /= val;
  resultMatrix.m11 /= val;
  resultMatrix.m12 /= val;
  resultMatrix.m20 /= val;
  resultMatrix.m21 /= val;
  resultMatrix.m22 /= val;

  return resultMatrix;
}

template<typename Real>
Matrix3x3<Real>& Matrix3x3<Real>::operator+=(const Matrix3x3& mat3)
{
  m00 += mat3.m00;
  m01 += mat3.m01;
  m02 += mat3.m02;
  m10 += mat3.m10;
  m11 += mat3.m11;
  m12 += mat3.m12;
  m20 += mat3.m20;
  m21 += mat3.m21;
  m22 += mat3.m22;

  return *this;
}

template<typename Real>
Matrix3x3<Real>& Matrix3x3<Real>::operator-=(const Matrix3x3& mat3)
{
  m00 -= mat3.m00;
  m01 -= mat3.m01;
  m02 -= mat3.m02;
  m10 -= mat3.m10;
  m11 -= mat3.m11;
  m12 -= mat3.m12;
  m20 -= mat3.m20;
  m21 -= mat3.m21;
  m22 -= mat3.m22;

  return *this;
}

template<typename Real>
Matrix3x3<Real>& Matrix3x3<Real>::operator*=(const Matrix3x3& mat3)
{
  *this = *this * mat3;

  return *this;
}

template<typename Real>
Matrix3x3<Real>& Matrix3x3<Real>::operator/=(const Matrix3x3& mat3)
{
  m00 /= mat3.m00;
  m01 /= mat3.m01;
  m02 /= mat3.m02;
  m10 /= mat3.m10;
  m11 /= mat3.m11;
  m12 /= mat3.m12;
  m20 /= mat3.m20;
  m21 /= mat3.m21;
  m22 /= mat3.m22;

  return *this;
}

template<typename Real>
Matrix3x3<Real>& Matrix3x3<Real>::operator+=(const Real val)
{
  m00 += val;
  m01 += val;
  m02 += val;
  m10 += val;
  m11 += val;
  m12 += val;
  m20 += val;
  m21 += val;
  m22 += val;

  return *this;
}

template<typename Real>
Matrix3x3<Real>& Matrix3x3<Real>::operator-=(const Real val)
{
  m00 -= val;
  m01 -= val;
  m02 -= val;
  m10 -= val;
  m11 -= val;
  m12 -= val;
  m20 -= val;
  m21 -= val;
  m22 -= val;

  return *this;
}

template<typename Real>
Matrix3x3<Real>& Matrix3x3<Real>::operator*=(const Real val)
{
  m00 *= val;
  m01 *= val;
  m02 *= val;
  m10 *= val;
  m11 *= val;
  m12 *= val;
  m20 *= val;
  m21 *= val;
  m22 *= val;

  return *this;
}

template<typename Real>
Matrix3x3<Real>& Matrix3x3<Real>::operator/=(const Real val)
{
  m00 /= val;
  m01 /= val;
  m02 /= val;
  m10 /= val;
  m11 /= val;
  m12 /= val;
  m20 /= val;
  m21 /= val;
  m22 /= val;

  return *this;
}

template<typename Real>
Vector3<Real> Matrix3x3<Real>::operator*(const Vector3<Real>& vec3) const
{
  Vector3<Real> resultVector;

  resultVector.x = vec3.x * m00 + vec3.y * m01 + vec3.z * m02;
  resultVector.y = vec3.x * m10 + vec3.y * m11 + vec3.z * m12;
  resultVector.z = vec3.x * m20 + vec3.y * m21 + vec3.z * m22;

  return resultVector;
}

template<typename Real>
Real Matrix3x3<Real>::GetElement(const unsigned int row, const unsigned int col) const
{
  return data[row][col];
}

template<typename Real>
Real& Matrix3x3<Real>::GetElement(const unsigned int row, const unsigned int col)
{
  return data[row][col];
}

template<typename Real>
Vector3<Real> Matrix3x3<Real>::GetRow(const unsigned int row) const
{
  return Vector3<Real>(data[row][0], data[row][1], data[row][2]);
}

template<typename Real>
Vector3<Real> Matrix3x3<Real>::GetColumn(const unsigned int col) const
{
  return Vector3<Real>(data[0][col], data[1][col], data[2][col]);
}

template<typename Real>
Matrix3x3<Real> Matrix3x3<Real>::MultiplyElementWise(const Matrix3x3& mat3)
{
  Matrix3x3 resultMatrix = *this;

  resultMatrix.m00 *= mat3.m00;
  resultMatrix.m01 *= mat3.m01;
  resultMatrix.m02 *= mat3.m02;
  resultMatrix.m10 *= mat3.m10;
  resultMatrix.m11 *= mat3.m11;
  resultMatrix.m12 *= mat3.m12;
  resultMatrix.m20 *= mat3.m20;
  resultMatrix.m21 *= mat3.m21;
  resultMatrix.m22 *= mat3.m22;

  return resultMatrix;
}

template<typename Real>
Matrix3x3<Real> Matrix3x3<Real>::GetTranspose() const
{
  return Matrix3x3(m00, m10, m20, m01, m11, m21, m02, m12, m22);
}

template<typename Real>
Matrix3x3<Real>& Matrix3x3<Real>::Transpose()
{
  return *this = GetTranspose();
}

template<typename Real>
Matrix3x3<Real>& Matrix3x3<Real>::Orthonormalize()
{
  Vector3<Real> v0 = GetColumn(0);
  Vector3<Real> v1 = GetColumn(1);
  Vector3<Real> v2 = GetColumn(2);

  Vector3<Real>::Orthonormalize(v0, v1, v2);

  m00 = v0.x;
  m10 = v0.y;
  m20 = v0.z;
  m01 = v1.x;
  m11 = v1.y;
  m21 = v1.z;
  m02 = v2.x;
  m12 = v2.y;
  m22 = v2.z;

  return *this;
}

#endif
