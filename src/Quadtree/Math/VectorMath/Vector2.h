/*******************************************************************************
* File:           Vector2.h
* Author:         Christian Alfons
* Date created:   2009-07-19
* Date modified:  2009-07-20
* Description:    A simple two-element vector class.
*******************************************************************************/

#ifndef VECTOR2_H_INCLUDED
#define VECTOR2_H_INCLUDED

// Represents a two-element vector
template<typename Real>
class Vector2
{
  public:
    // Instance variables
    union
    {
      Real data[2];

      struct
      {
        Real x;
        Real y;
      };

      struct
      {
        Real u;
        Real v;
      };
    };

    // Constructors
    inline Vector2();
    inline Vector2(const Real x0, const Real y0);
    inline Vector2(const Real val);
    inline Vector2(const Real* vals);

    // Operators
    inline Vector2  operator-() const;
    inline Real     operator[](const unsigned int index) const;
    inline Real&    operator[](const unsigned int index);
    inline bool     operator==(const Vector2& vec2) const;
    inline bool     operator!=(const Vector2& vec2) const;
    inline Vector2  operator+(const Vector2& vec2) const;
    inline Vector2  operator-(const Vector2& vec2) const;
    inline Vector2  operator*(const Vector2& vec2) const;
    inline Vector2  operator/(const Vector2& vec2) const;
    inline Vector2  operator+(const Real val) const;
    inline Vector2  operator-(const Real val) const;
    inline Vector2  operator*(const Real val) const;
    inline Vector2  operator/(const Real val) const;
    inline Vector2& operator+=(const Vector2& vec2);
    inline Vector2& operator-=(const Vector2& vec2);
    inline Vector2& operator*=(const Vector2& vec2);
    inline Vector2& operator/=(const Vector2& vec2);
    inline Vector2& operator+=(const Real val);
    inline Vector2& operator-=(const Real val);
    inline Vector2& operator*=(const Real val);
    inline Vector2& operator/=(const Real val);

    // Instance methods
    inline Real     Dot(const Vector2& vec2) const;
    inline Real     GetSquaredLength() const;
    inline Real     GetLength() const;
    inline Vector2  GetNormalized() const;
    inline Vector2& Normalize();
};

template<typename Real>
Vector2<Real>::Vector2()
{
  x = y = Real(0.0);
}

template<typename Real>
Vector2<Real>::Vector2(const Real x0, const Real y0)
{
  x = x0;
  y = y0;
}

template<typename Real>
Vector2<Real>::Vector2(const Real val)
{
  x = y = val;
}

template<typename Real>
Vector2<Real>::Vector2(const Real* vals)
{
  x = vals[0];
  y = vals[1];
}

template<typename Real>
Vector2<Real> Vector2<Real>::operator-() const
{
  return Vector2<Real>(-x, -y);
}

template<typename Real>
Real Vector2<Real>::operator[](const unsigned int index) const
{
  return data[index];
}

template<typename Real>
Real& Vector2<Real>::operator[](const unsigned int index)
{
  return data[index];
}

template<typename Real>
bool Vector2<Real>::operator==(const Vector2& vec2) const
{
  return x == vec2.x && y == vec2.y;
}

template<typename Real>
bool Vector2<Real>::operator!=(const Vector2& vec2) const
{
  return !(*this == vec2);
}

template<typename Real>
Vector2<Real> Vector2<Real>::operator+(const Vector2& vec2) const
{
  return Vector2(x + vec2.x, y + vec2.y);
}

template<typename Real>
Vector2<Real> Vector2<Real>::operator-(const Vector2& vec2) const
{
  return Vector2(x - vec2.x, y - vec2.y);
}

template<typename Real>
Vector2<Real> Vector2<Real>::operator*(const Vector2& vec2) const
{
  return Vector2(x * vec2.x, y * vec2.y);
}

template<typename Real>
Vector2<Real> Vector2<Real>::operator/(const Vector2& vec2) const
{
  return Vector2(x / vec2.x, y / vec2.y);
}

template<typename Real>
Vector2<Real> Vector2<Real>::operator+(const Real val) const
{
  return Vector2(x + val, y + val);
}

template<typename Real>
Vector2<Real> Vector2<Real>::operator-(const Real val) const
{
  return Vector2(x - val, y - val);
}

template<typename Real>
Vector2<Real> Vector2<Real>::operator*(const Real val) const
{
  return Vector2(x * val, y * val);
}

template<typename Real>
Vector2<Real> Vector2<Real>::operator/(const Real val) const
{
  return Vector2(x / val, y / val);
}

template<typename Real>
Vector2<Real>& Vector2<Real>::operator+=(const Vector2& vec2)
{
  x += vec2.x;
  y += vec2.y;

  return *this;
}

template<typename Real>
Vector2<Real>& Vector2<Real>::operator-=(const Vector2& vec2)
{
  x -= vec2.x;
  y -= vec2.y;

  return *this;
}

template<typename Real>
Vector2<Real>& Vector2<Real>::operator*=(const Vector2& vec2)
{
  x *= vec2.x;
  y *= vec2.y;

  return *this;
}

template<typename Real>
Vector2<Real>& Vector2<Real>::operator/=(const Vector2& vec2)
{
  x /= vec2.x;
  y /= vec2.y;

  return *this;
}

template<typename Real>
Vector2<Real>& Vector2<Real>::operator+=(const Real val)
{
  x += val;
  y += val;

  return *this;
}

template<typename Real>
Vector2<Real>& Vector2<Real>::operator-=(const Real val)
{
  x -= val;
  y -= val;

  return *this;
}

template<typename Real>
Vector2<Real>& Vector2<Real>::operator*=(const Real val)
{
  x *= val;
  y *= val;

  return *this;
}

template<typename Real>
Vector2<Real>& Vector2<Real>::operator/=(const Real val)
{
  x /= val;
  y /= val;

  return *this;
}

template<typename Real>
Real Vector2<Real>::Dot(const Vector2& vec2) const
{
  return x * vec2.x + y * vec2.y;
}

template<typename Real>
Real Vector2<Real>::GetSquaredLength() const
{
  return Dot(*this);
}

template<typename Real>
Real Vector2<Real>::GetLength() const
{
  return sqrt(GetSquaredLength());
}

template<typename Real>
Vector2<Real> Vector2<Real>::GetNormalized() const
{
  if (GetSquaredLength() == 0)
    return *this;
  return *this / GetLength();
}

template<typename Real>
Vector2<Real>& Vector2<Real>::Normalize()
{
  if (GetSquaredLength() == 0)
    return *this;
  return *this /= GetLength();
}

#endif
