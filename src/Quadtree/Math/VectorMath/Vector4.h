/*******************************************************************************
* File:           Vector4.h
* Author:         Christian Alfons
* Date created:   2009-07-17
* Date modified:  2009-07-20
* Description:    A simple four-element vector class, intended for use with
*                 homogeneous coordinates.
*******************************************************************************/

#ifndef VECTOR4_H_INCLUDED
#define VECTOR4_H_INCLUDED

// Represents a four-element vector
template<typename Real>
class Vector4
{
  public:
    // Instance variables
    union
    {
      Real data[4];

      struct
      {
        Real x;
        Real y;
        Real z;
        Real w;
      };

      struct
      {
        Real r;
        Real g;
        Real b;
        Real a;
      };
    };

    // Constructors
    inline Vector4();
    inline Vector4(const Real x0, const Real y0, const Real z0, const Real w0);
    inline Vector4(const Real val);
    inline Vector4(const Real* vals);

    // Operators
    inline Vector4  operator-() const;
    inline Real     operator[](const unsigned int index) const;
    inline Real&    operator[](const unsigned int index);
    inline bool     operator==(const Vector4& vec4) const;
    inline bool     operator!=(const Vector4& vec4) const;
    inline Vector4  operator+(const Vector4& vec4) const;
    inline Vector4  operator-(const Vector4& vec4) const;
    inline Vector4  operator*(const Vector4& vec4) const;
    inline Vector4  operator/(const Vector4& vec4) const;
    inline Vector4  operator+(const Real val) const;
    inline Vector4  operator-(const Real val) const;
    inline Vector4  operator*(const Real val) const;
    inline Vector4  operator/(const Real val) const;
    inline Vector4& operator+=(const Vector4& vec4);
    inline Vector4& operator-=(const Vector4& vec4);
    inline Vector4& operator*=(const Vector4& vec4);
    inline Vector4& operator/=(const Vector4& vec4);
    inline Vector4& operator+=(const Real val);
    inline Vector4& operator-=(const Real val);
    inline Vector4& operator*=(const Real val);
    inline Vector4& operator/=(const Real val);

    // Instance variables
    inline Real     Dot(const Vector4& vec4) const;
    inline Real     GetSquaredLength() const;
    inline Real     GetLength() const;
    inline Vector4  GetNormalized() const;
    inline Vector4& Normalize();
    inline Vector4& DivideByW();
};

template<typename Real>
Vector4<Real>::Vector4()
{
  x = y = z = w = Real(0.0);
}

template<typename Real>
Vector4<Real>::Vector4(const Real x0, const Real y0, const Real z0, const Real w0)
{
  x = x0;
  y = y0;
  z = z0;
  w = w0;
}

template<typename Real>
Vector4<Real>::Vector4(const Real val)
{
  x = y = z = w = val;
}

template<typename Real>
Vector4<Real>::Vector4(const Real* vals)
{
  x = vals[0];
  y = vals[1];
  z = vals[2];
  w = vals[3];
}

template<typename Real>
Vector4<Real> Vector4<Real>::operator-() const
{
  return Vector4<Real>(-x, -y, -z, -w);
}

template<typename Real>
Real Vector4<Real>::operator[](const unsigned int index) const
{
  return data[index];
}

template<typename Real>
Real& Vector4<Real>::operator[](const unsigned int index)
{
  return data[index];
}

template<typename Real>
bool Vector4<Real>::operator==(const Vector4& vec4) const
{
  return x == vec4.x && y == vec4.y && z == vec4.z && w = vec4.w;
}

template<typename Real>
bool Vector4<Real>::operator!=(const Vector4& vec4) const
{
  return !(*this == vec4);
}

template<typename Real>
Vector4<Real> Vector4<Real>::operator+(const Vector4& vec4) const
{
  return Vector4(x + vec4.x, y + vec4.y, z + vec4.z, w + vec4.w);
}

template<typename Real>
Vector4<Real> Vector4<Real>::operator-(const Vector4& vec4) const
{
  return Vector4(x - vec4.x, y - vec4.y, z - vec4.z, w - vec4.w);
}

template<typename Real>
Vector4<Real> Vector4<Real>::operator*(const Vector4& vec4) const
{
  return Vector4(x * vec4.x, y * vec4.y, z * vec4.z, w * vec4.w);
}

template<typename Real>
Vector4<Real> Vector4<Real>::operator/(const Vector4& vec4) const
{
  return Vector4(x / vec4.x, y / vec4.y, z / vec4.z, w / vec4.w);
}

template<typename Real>
Vector4<Real> Vector4<Real>::operator+(const Real val) const
{
  return Vector4(x + val, y + val, z + val, w + val);
}

template<typename Real>
Vector4<Real> Vector4<Real>::operator-(const Real val) const
{
  return Vector4(x - val, y - val, z - val, w - val);
}

template<typename Real>
Vector4<Real> Vector4<Real>::operator*(const Real val) const
{
  return Vector4(x * val, y * val, z * val, w * val);
}

template<typename Real>
Vector4<Real> Vector4<Real>::operator/(const Real val) const
{
  return Vector4(x / val, y / val, z / val, w / val);
}

template<typename Real>
Vector4<Real>& Vector4<Real>::operator+=(const Vector4& vec4)
{
  x += vec4.x;
  y += vec4.y;
  z += vec4.z;
  w += vec4.w;

  return *this;
}

template<typename Real>
Vector4<Real>& Vector4<Real>::operator-=(const Vector4& vec4)
{
  x -= vec4.x;
  y -= vec4.y;
  z -= vec4.z;
  w -= vec4.w;

  return *this;
}

template<typename Real>
Vector4<Real>& Vector4<Real>::operator*=(const Vector4& vec4)
{
  x *= vec4.x;
  y *= vec4.y;
  z *= vec4.z;
  w *= vec4.w;

  return *this;
}

template<typename Real>
Vector4<Real>& Vector4<Real>::operator/=(const Vector4& vec4)
{
  x /= vec4.x;
  y /= vec4.y;
  z /= vec4.z;
  w /= vec4.w;

  return *this;
}

template<typename Real>
Vector4<Real>& Vector4<Real>::operator+=(const Real val)
{
  x += val;
  y += val;
  z += val;
  w += val;

  return *this;
}

template<typename Real>
Vector4<Real>& Vector4<Real>::operator-=(const Real val)
{
  x -= val;
  y -= val;
  z -= val;
  w -= val;

  return *this;
}

template<typename Real>
Vector4<Real>& Vector4<Real>::operator*=(const Real val)
{
  x *= val;
  y *= val;
  z *= val;
  w *= val;

  return *this;
}

template<typename Real>
Vector4<Real>& Vector4<Real>::operator/=(const Real val)
{
  x /= val;
  y /= val;
  z /= val;
  w /= val;

  return *this;
}

template<typename Real>
Real Vector4<Real>::Dot(const Vector4& vec4) const
{
  return x * vec4.x + y * vec4.y + z * vec4.z + w * vec4.w;
}

template<typename Real>
Real Vector4<Real>::GetSquaredLength() const
{
  return Dot(*this);
}

template<typename Real>
Real Vector4<Real>::GetLength() const
{
  return sqrt(GetSquaredLength());
}

template<typename Real>
Vector4<Real> Vector4<Real>::GetNormalized() const
{
  if (GetSquaredLength() == 0)
    return *this;
  return *this / GetLength();
}

template<typename Real>
Vector4<Real>& Vector4<Real>::Normalize()
{
  if (GetSquaredLength() == 0)
    return *this;
  return *this /= GetLength();
}

template<typename Real>
Vector4<Real>& Vector4<Real>::DivideByW()
{
  if (w == 0)
    return *this;
  return *this /= w;
}

#endif
