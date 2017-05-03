/*******************************************************************************
* File:           Vector3.h
* Author:         Christian Alfons
* Date created:   2009-07-17
* Date modified:  2009-11-12
* Description:    A simple three-element vector class.
*******************************************************************************/

#ifndef VECTOR3_H_INCLUDED
#define VECTOR3_H_INCLUDED

#include <cmath>

// Represents a three-element vector
template<typename Real>
class Vector3
{
  public:
    // Instance variables
    union
    {
      Real data[3];

      struct
      {
        Real x;
        Real y;
        Real z;
      };

      struct
      {
        Real r;
        Real g;
        Real b;
      };

      struct
      {
        Real u;
        Real v;
        Real w;
      };
    };

    // Class methods
    static inline void Orthonormalize(Vector3& v0, Vector3& v1, Vector3& v2);

    // Constructors
    inline Vector3();
    inline Vector3(const Real x0, const Real y0, const Real z0);
    inline Vector3(const Real val);
    inline Vector3(const Real* vals);

    // Operators
    inline Vector3  operator-() const;
    inline Real     operator[](const unsigned int index) const;
    inline Real&    operator[](const unsigned int index);
    inline bool     operator>(const Vector3<Real>& vec3) const;
    inline bool     operator<(const Vector3<Real>& vec3) const;
    inline bool     operator==(const Vector3& vec3) const;
    inline bool     operator!=(const Vector3& vec3) const;
    inline Vector3  operator+(const Vector3& vec3) const;
    inline Vector3  operator-(const Vector3& vec3) const;
    inline Vector3  operator*(const Vector3& vec3) const;
    inline Vector3  operator/(const Vector3& vec3) const;
    inline Vector3  operator+(const Real val) const;
    inline Vector3  operator-(const Real val) const;
    inline Vector3  operator*(const Real val) const;
    inline Vector3  operator/(const Real val) const;
    inline Vector3& operator+=(const Vector3& vec3);
    inline Vector3& operator-=(const Vector3& vec3);
    inline Vector3& operator*=(const Vector3& vec3);
    inline Vector3& operator/=(const Vector3& vec3);
    inline Vector3& operator+=(const Real val);
    inline Vector3& operator-=(const Real val);
    inline Vector3& operator*=(const Real val);
    inline Vector3& operator/=(const Real val);

    // Instance methods
    inline Real     Dot(const Vector3& vec3) const;
    inline Vector3  Cross(const Vector3& vec3) const;
    inline Vector3  ProjectOntoVector(const Vector3& vec3) const;
    inline Real     GetSquaredLength() const;
    inline Real     GetLength() const;
    inline Vector3  GetNormalized() const;
    inline Vector3& Normalize();
};

template<typename Real>
void Vector3<Real>::Orthonormalize(Vector3& v0, Vector3& v1, Vector3& v2)
{
  Vector3 u1 = v1 - v1.ProjectOntoVector(v0);
  Vector3 u2 = v2 - v1.ProjectOntoVector(v0) - v2.ProjectOntoVector(u1);

  v0.Normalize();
  v1 = u1.Normalize();
  v2 = u2.Normalize();
}

template<typename Real>
Vector3<Real>::Vector3()
{
  x = y = z = Real(0.0);
}

template<typename Real>
Vector3<Real>::Vector3(const Real x0, const Real y0, const Real z0)
{
  x = x0;
  y = y0;
  z = z0;
}

template<typename Real>
Vector3<Real>::Vector3(const Real val)
{
  x = y = z = val;
}

template<typename Real>
Vector3<Real>::Vector3(const Real* vals)
{
  x = vals[0];
  y = vals[1];
  z = vals[2];
}

template<typename Real>
Vector3<Real> Vector3<Real>::operator-() const
{
  return Vector3<Real>(-x, -y, -z);
}

template<typename Real>
Real Vector3<Real>::operator[](const unsigned int index) const
{
  return data[index];
}

template<typename Real>
Real& Vector3<Real>::operator[](const unsigned int index)
{
  return data[index];
}

template<typename Real>
bool Vector3<Real>::operator>(const Vector3<Real>& vec3) const
{
  if (data[0] > vec3[0])
  {
    return true;
  }
  else if (data[0] < vec3[0])
  {
    return false;
  }
  else if (data[1] > vec3[1])
  {
    return true;
  }
  else if (data[1] < vec3[1])
  {
    return false;
  }
  else if (data[2] > vec3[2])
  {
    return true;
  }

  return false;
}

template<typename Real>
bool Vector3<Real>::operator<(const Vector3<Real>& vec3) const
{
  if (data[0] < vec3[0])
  {
    return true;
  }
  else if (data[0] > vec3[0])
  {
    return false;
  }
  else if (data[1] < vec3[1])
  {
    return true;
  }
  else if (data[1] > vec3[1])
  {
    return false;
  }
  else if (data[2] < vec3[2])
  {
    return true;
  }

  return false;
}

template<typename Real>
bool Vector3<Real>::operator==(const Vector3& vec3) const
{
  return x == vec3.x && y == vec3.y && z == vec3.z;
}

template<typename Real>
bool Vector3<Real>::operator!=(const Vector3& vec3) const
{
  return !(*this == vec3);
}

template<typename Real>
Vector3<Real> Vector3<Real>::operator+(const Vector3& vec3) const
{
  return Vector3(x + vec3.x, y + vec3.y, z + vec3.z);
}

template<typename Real>
Vector3<Real> Vector3<Real>::operator-(const Vector3& vec3) const
{
  return Vector3(x - vec3.x, y - vec3.y, z - vec3.z);
}

template<typename Real>
Vector3<Real> Vector3<Real>::operator*(const Vector3& vec3) const
{
  return Vector3(x * vec3.x, y * vec3.y, z * vec3.z);
}

template<typename Real>
Vector3<Real> Vector3<Real>::operator/(const Vector3& vec3) const
{
  return Vector3(x / vec3.x, y / vec3.y, z / vec3.z);
}

template<typename Real>
Vector3<Real> Vector3<Real>::operator+(const Real val) const
{
  return Vector3(x + val, y + val, z + val);
}

template<typename Real>
Vector3<Real> Vector3<Real>::operator-(const Real val) const
{
  return Vector3(x - val, y - val, z - val);
}

template<typename Real>
Vector3<Real> Vector3<Real>::operator*(const Real val) const
{
  return Vector3(x * val, y * val, z * val);
}

template<typename Real>
Vector3<Real> Vector3<Real>::operator/(const Real val) const
{
  return Vector3(x / val, y / val, z / val);
}

template<typename Real>
Vector3<Real>& Vector3<Real>::operator+=(const Vector3& vec3)
{
  x += vec3.x;
  y += vec3.y;
  z += vec3.z;

  return *this;
}

template<typename Real>
Vector3<Real>& Vector3<Real>::operator-=(const Vector3& vec3)
{
  x -= vec3.x;
  y -= vec3.y;
  z -= vec3.z;

  return *this;
}

template<typename Real>
Vector3<Real>& Vector3<Real>::operator*=(const Vector3& vec3)
{
  x *= vec3.x;
  y *= vec3.y;
  z *= vec3.z;

  return *this;
}

template<typename Real>
Vector3<Real>& Vector3<Real>::operator/=(const Vector3& vec3)
{
  x /= vec3.x;
  y /= vec3.y;
  z /= vec3.z;

  return *this;
}

template<typename Real>
Vector3<Real>& Vector3<Real>::operator+=(const Real val)
{
  x += val;
  y += val;
  z += val;

  return *this;
}

template<typename Real>
Vector3<Real>& Vector3<Real>::operator-=(const Real val)
{
  x -= val;
  y -= val;
  z -= val;

  return *this;
}

template<typename Real>
Vector3<Real>& Vector3<Real>::operator*=(const Real val)
{
  x *= val;
  y *= val;
  z *= val;

  return *this;
}

template<typename Real>
Vector3<Real>& Vector3<Real>::operator/=(const Real val)
{
  x /= val;
  y /= val;
  z /= val;

  return *this;
}

template<typename Real>
Real Vector3<Real>::Dot(const Vector3& vec3) const
{
  return x * vec3.x + y * vec3.y + z * vec3.z;
}

template<typename Real>
Vector3<Real> Vector3<Real>::Cross(const Vector3& vec3) const
{
  return Vector3<Real>(y * vec3.z - z * vec3.y, z * vec3.x - x * vec3.z, x * vec3.y - y * vec3.x);
}

template<typename Real>
Vector3<Real> Vector3<Real>::ProjectOntoVector(const Vector3& vec3) const
{
  return vec3 * Dot(vec3) / vec3.Dot(vec3);
}

template<typename Real>
Real Vector3<Real>::GetSquaredLength() const
{
  return Dot(*this);
}

template<typename Real>
Real Vector3<Real>::GetLength() const
{
  return sqrt(GetSquaredLength());
}

template<typename Real>
Vector3<Real> Vector3<Real>::GetNormalized() const
{
  if (GetSquaredLength() == 0)
    return *this;
  return *this / GetLength();
}

template<typename Real>
Vector3<Real>& Vector3<Real>::Normalize()
{
  if (GetSquaredLength() == 0)
    return *this;
  return *this /= GetLength();
}

#endif
