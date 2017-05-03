/*******************************************************************************
* File:           Matrix2x2.h
* Author:         Christian Alfons
* Date created:   2009-07-20
* Date modified:  2009-07-20
* Description:    A simple two-by-two matrix class.
*******************************************************************************/

#ifndef MATRIX2X2_H_INCLUDED
#define MATRIX2X2_H_INCLUDED

// Represents a two-by-two matrix
template<typename Real>
class Matrix2x2
{
  public:
    // Instance variables
    union
    {
      Real data[2][2];
      Real data1d[4];

      struct
      {
        Real m00;
        Real m01;
        Real m10;
        Real m11;
      };
    };

    // Class methods
    static inline Matrix2x2 CreateZeroMatrix();
    static inline Matrix2x2 CreateIdentityMatrix();
    static inline Matrix2x2 CreateScalingMatrix(const Real scale);
    static inline Matrix2x2 CreateScalingMatrix(const Real scaleX, const Real scaleY);
    static inline Matrix2x2 CreateRotationMatrix(const Real radians);

    // Constructors
    inline Matrix2x2();
    inline Matrix2x2(const Real n00, const Real n01,
                     const Real n10, const Real n11);
    inline Matrix2x2(const Real val);
    inline Matrix2x2(const Real* vals);

    // Operators
    inline Real          operator[](const unsigned int index) const;
    inline Real&         operator[](const unsigned int index);
    inline bool          operator==(const Matrix2x2& mat2) const;
    inline bool          operator!=(const Matrix2x2& mat2) const;
    inline Matrix2x2     operator+(const Matrix2x2& mat2) const;
    inline Matrix2x2     operator-(const Matrix2x2& mat2) const;
    inline Matrix2x2     operator*(const Matrix2x2& mat2) const;
    inline Matrix2x2     operator/(const Matrix2x2& mat2) const;
    inline Matrix2x2     operator+(const Real val) const;
    inline Matrix2x2     operator-(const Real val) const;
    inline Matrix2x2     operator*(const Real val) const;
    inline Matrix2x2     operator/(const Real val) const;
    inline Matrix2x2&    operator+=(const Matrix2x2& mat2);
    inline Matrix2x2&    operator-=(const Matrix2x2& mat2);
    inline Matrix2x2&    operator*=(const Matrix2x2& mat2);
    inline Matrix2x2&    operator/=(const Matrix2x2& mat2);
    inline Matrix2x2&    operator+=(const Real val);
    inline Matrix2x2&    operator-=(const Real val);
    inline Matrix2x2&    operator*=(const Real val);
    inline Matrix2x2&    operator/=(const Real val);
    inline Vector2<Real> operator*(const Vector2<Real>& vec2) const;
    inline Real          GetElement(const unsigned int row, const unsigned int col) const;
    inline Real&         GetElement(const unsigned int row, const unsigned int col);
    inline Vector2<Real> GetRow(const unsigned int row) const;
    inline Vector2<Real> GetColumn(const unsigned int col) const;

    // Instance methods
    inline Matrix2x2  MultiplyElementWise(const Matrix2x2& matrix);
    inline Matrix2x2  GetTranspose() const;
    inline Matrix2x2& Transpose();
};

template<typename Real>
Matrix2x2<Real> Matrix2x2<Real>::CreateZeroMatrix()
{
  return Matrix2x2(Real(0.0));
}

template<typename Real>
Matrix2x2<Real> Matrix2x2<Real>::CreateIdentityMatrix()
{
  Matrix2x2 identityMatrix(Real(0.0));

  identityMatrix.m00 = identityMatrix.m11 = Real(1.0);

  return identityMatrix;
}

template<typename Real>
Matrix2x2<Real> Matrix2x2<Real>::CreateScalingMatrix(const Real scale)
{
  Matrix2x2 scalingMatrix;

  scalingMatrix.m00 = scalingMatrix.m11 = scalingMatrix.m22 = scale;

  return scalingMatrix;
}

template<typename Real>
Matrix2x2<Real> Matrix2x2<Real>::CreateScalingMatrix(const Real scaleX, const Real scaleY)
{
  Matrix2x2 scalingMatrix;

  scalingMatrix.m00 = scaleX;
  scalingMatrix.m11 = scaleY;

  return scalingMatrix;
}

template<typename Real>
Matrix2x2<Real> Matrix2x2<Real>::CreateRotationMatrix(const Real radians)
{
  Matrix2x2 rotationMatrix;

  rotationMatrix.m00 =  cos(radians);
  rotationMatrix.m01 = -sin(radians);
  rotationMatrix.m10 =  sin(radians);
  rotationMatrix.m11 =  cos(radians);

  return rotationMatrix;
}

template<typename Real>
Matrix2x2<Real>::Matrix2x2()
{
  *this = CreateIdentityMatrix();
}

template<typename Real>
Matrix2x2<Real>::Matrix2x2(const Real n00, const Real n01,
                           const Real n10, const Real n11)
{
  m00 = n00;
  m01 = n01;
  m10 = n10;
  m11 = n11;
}

template<typename Real>
Matrix2x2<Real>::Matrix2x2(const Real val)
{
  m00 = m01 = m10 = m11 = val;
}

template<typename Real>
Matrix2x2<Real>::Matrix2x2(const Real* vals)
{
  data1d[0]  = vals[0];
  data1d[1]  = vals[1];
  data1d[2]  = vals[2];
  data1d[3]  = vals[3];
}

template<typename Real>
Real Matrix2x2<Real>::operator[](const unsigned int index) const
{
  return data1d[index];
}

template<typename Real>
Real& Matrix2x2<Real>::operator[](const unsigned int index)
{
  return data1d[index];
}

template<typename Real>
bool Matrix2x2<Real>::operator==(const Matrix2x2& mat2) const
{
  return m00 == mat2.m00 && m01 == mat2.m01
      && m10 == mat2.m10 && m11 == mat2.m11;
}

template<typename Real>
bool Matrix2x2<Real>::operator!=(const Matrix2x2& mat2) const
{
  return !(*this == mat2);
}

template<typename Real>
Matrix2x2<Real> Matrix2x2<Real>::operator+(const Matrix2x2& mat2) const
{
  Matrix2x2 resultMatrix = *this;

  resultMatrix.m00 += mat2.m00;
  resultMatrix.m01 += mat2.m01;
  resultMatrix.m10 += mat2.m10;
  resultMatrix.m11 += mat2.m11;

  return resultMatrix;
}

template<typename Real>
Matrix2x2<Real> Matrix2x2<Real>::operator-(const Matrix2x2& mat2) const
{
  Matrix2x2 resultMatrix = *this;

  resultMatrix.m00 -= mat2.m00;
  resultMatrix.m01 -= mat2.m01;
  resultMatrix.m10 -= mat2.m10;
  resultMatrix.m11 -= mat2.m11;

  return resultMatrix;
}

template<typename Real>
Matrix2x2<Real> Matrix2x2<Real>::operator*(const Matrix2x2& mat2) const
{
  Matrix2x2 resultMatrix;

  resultMatrix.m00 = m00 * mat2.m00 + m01 * mat2.m10;
  resultMatrix.m01 = m00 * mat2.m01 + m01 * mat2.m11;
  resultMatrix.m10 = m10 * mat2.m00 + m11 * mat2.m10;
  resultMatrix.m11 = m10 * mat2.m01 + m11 * mat2.m11;

  return resultMatrix;
}

template<typename Real>
Matrix2x2<Real> Matrix2x2<Real>::operator/(const Matrix2x2& mat2) const
{
  Matrix2x2 resultMatrix = *this;

  resultMatrix.m00 /= mat2.m00;
  resultMatrix.m01 /= mat2.m01;
  resultMatrix.m10 /= mat2.m10;
  resultMatrix.m11 /= mat2.m11;

  return resultMatrix;
}

template<typename Real>
Matrix2x2<Real> Matrix2x2<Real>::operator+(const Real val) const
{
  Matrix2x2 resultMatrix = *this;

  resultMatrix.m00 += val;
  resultMatrix.m01 += val;
  resultMatrix.m10 += val;
  resultMatrix.m11 += val;

  return resultMatrix;
}

template<typename Real>
Matrix2x2<Real> Matrix2x2<Real>::operator-(const Real val) const
{
  Matrix2x2 resultMatrix = *this;

  resultMatrix.m00 -= val;
  resultMatrix.m01 -= val;
  resultMatrix.m10 -= val;
  resultMatrix.m11 -= val;

  return resultMatrix;
}

template<typename Real>
Matrix2x2<Real> Matrix2x2<Real>::operator*(const Real val) const
{
  Matrix2x2 resultMatrix = *this;

  resultMatrix.m00 *= val;
  resultMatrix.m01 *= val;
  resultMatrix.m10 *= val;
  resultMatrix.m11 *= val;

  return resultMatrix;
}

template<typename Real>
Matrix2x2<Real> Matrix2x2<Real>::operator/(const Real val) const
{
  Matrix2x2 resultMatrix = *this;

  resultMatrix.m00 /= val;
  resultMatrix.m01 /= val;
  resultMatrix.m10 /= val;
  resultMatrix.m11 /= val;

  return resultMatrix;
}

template<typename Real>
Matrix2x2<Real>& Matrix2x2<Real>::operator+=(const Matrix2x2& mat2)
{
  m00 += mat2.m00;
  m01 += mat2.m01;
  m10 += mat2.m10;
  m11 += mat2.m11;

  return *this;
}

template<typename Real>
Matrix2x2<Real>& Matrix2x2<Real>::operator-=(const Matrix2x2& mat2)
{
  m00 -= mat2.m00;
  m01 -= mat2.m01;
  m10 -= mat2.m10;
  m11 -= mat2.m11;

  return *this;
}

template<typename Real>
Matrix2x2<Real>& Matrix2x2<Real>::operator*=(const Matrix2x2& mat2)
{
  *this = *this * mat2;

  return *this;
}

template<typename Real>
Matrix2x2<Real>& Matrix2x2<Real>::operator/=(const Matrix2x2& mat2)
{
  m00 /= mat2.m00;
  m01 /= mat2.m01;
  m10 /= mat2.m10;
  m11 /= mat2.m11;

  return *this;
}

template<typename Real>
Matrix2x2<Real>& Matrix2x2<Real>::operator+=(const Real val)
{
  m00 += val;
  m01 += val;
  m10 += val;
  m11 += val;

  return *this;
}

template<typename Real>
Matrix2x2<Real>& Matrix2x2<Real>::operator-=(const Real val)
{
  m00 -= val;
  m01 -= val;
  m10 -= val;
  m11 -= val;

  return *this;
}

template<typename Real>
Matrix2x2<Real>& Matrix2x2<Real>::operator*=(const Real val)
{
  m00 *= val;
  m01 *= val;
  m10 *= val;
  m11 *= val;

  return *this;
}

template<typename Real>
Matrix2x2<Real>& Matrix2x2<Real>::operator/=(const Real val)
{
  m00 /= val;
  m01 /= val;
  m10 /= val;
  m11 /= val;

  return *this;
}

template<typename Real>
Vector2<Real> Matrix2x2<Real>::operator*(const Vector2<Real>& vec2) const
{
  Vector2<Real> resultVector;

  resultVector.x = vec2.x * m00 + vec2.y * m01;
  resultVector.y = vec2.x * m10 + vec2.y * m11;

  return resultVector;
}

template<typename Real>
Real Matrix2x2<Real>::GetElement(const unsigned int row, const unsigned int col) const
{
  return data[row][col];
}

template<typename Real>
Real& Matrix2x2<Real>::GetElement(const unsigned int row, const unsigned int col)
{
  return data[row][col];
}

template<typename Real>
Vector2<Real> Matrix2x2<Real>::GetRow(const unsigned int row) const
{
  return Vector2<Real>(data[row][0], data[row][1]);
}

template<typename Real>
Vector2<Real> Matrix2x2<Real>::GetColumn(const unsigned int col) const
{
  return Vector2<Real>(data[0][col], data[1][col]);
}

template<typename Real>
Matrix2x2<Real> Matrix2x2<Real>::MultiplyElementWise(const Matrix2x2& mat2)
{
  Matrix2x2 resultMatrix = *this;

  resultMatrix.m00 *= mat2.m00;
  resultMatrix.m01 *= mat2.m01;
  resultMatrix.m10 *= mat2.m10;
  resultMatrix.m11 *= mat2.m11;

  return resultMatrix;
}

template<typename Real>
Matrix2x2<Real> Matrix2x2<Real>::GetTranspose() const
{
  return Matrix2x2(m00, m10, m01, m11);
}

template<typename Real>
Matrix2x2<Real>& Matrix2x2<Real>::Transpose()
{
  return *this = GetTranspose();
}

#endif
