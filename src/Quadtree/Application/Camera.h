/*******************************************************************************
* File:           Camera.h
* Author:         Christian Alfons
* Date created:   2009-07-21
* Date modified:  2011-02-17
* Description:    A simple (first-person) camera class for OpenGL. Stores and
*                 updates an orientation matrix, which is handy when there's no
*                 definite up direction (like space travel).
*******************************************************************************/

#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include "../Math/VectorMath.h"

template<typename Real>
class Camera
{
  private:
    //Instance variables
    Matrix3x3<Real> orientation;
    Vector3<Real>   position;

  public:
    // Constructors
    Camera(const Vector3<Real> &position = Vector3<Real>(0.0, 0.0, 0.0), const Matrix3x3<Real> &orientation = Matrix3x3<Real>::CreateIdentityMatrix());

    // Instance methods
    Matrix3x3<Real> GetOrientation() const;
    void            SetOrientation(const Matrix3x3<Real> &orientation);
    Vector3<Real>   GetPosition() const;
    void            SetPosition(const Vector3<Real> &position);
    void            RotateEuler(const Vector3<Real> &radians);
    void            Rotate(const Matrix3x3<Real> &orientation);
    void            Translate(const Vector3<Real> &translation);
    Vector3<Real>   GetForwardVector() const;
    Vector3<Real>   GetUpVector() const;
    Vector3<Real>   GetLeftVector() const;
    void            Use() const;
};

template<typename Real>
Camera<Real>::Camera(const Vector3<Real> &position, const Matrix3x3<Real> &orientation)
{
  SetPosition(position);
  SetOrientation(orientation);
}

template<typename Real>
Matrix3x3<Real> Camera<Real>::GetOrientation() const
{
  return orientation;
}

template<typename Real>
void Camera<Real>::SetOrientation(const Matrix3x3<Real> &orientation)
{
  this->orientation = orientation;
}

template<typename Real>
Vector3<Real> Camera<Real>::GetPosition() const
{
  return position;
}

template<typename Real>
void Camera<Real>::SetPosition(const Vector3<Real> &position)
{
  this->position = position;
}

template<typename Real>
void Camera<Real>::RotateEuler(const Vector3<Real> &radians)
{
  Rotate(Matrix3x3<Real>::CreateRotationMatrixY(radians.y)
       * Matrix3x3<Real>::CreateRotationMatrixX(radians.x)
       * Matrix3x3<Real>::CreateRotationMatrixZ(radians.z));
}

template<typename Real>
void Camera<Real>::Rotate(const Matrix3x3<Real> &rotation)
{
  orientation *= rotation;
  orientation.Orthonormalize();
}

template<typename Real>
void Camera<Real>::Translate(const Vector3<Real> &translation)
{
  position += orientation * translation;
}

template<typename Real>
Vector3<Real> Camera<Real>::GetForwardVector() const
{
  return orientation.GetColumn(2);
}

template<typename Real>
Vector3<Real> Camera<Real>::GetUpVector() const
{
  return orientation.GetColumn(1);
}

template<typename Real>
Vector3<Real> Camera<Real>::GetLeftVector() const
{
  return orientation.GetColumn(0);
}

template<typename Real>
void Camera<Real>::Use() const
{
  const Vector3<Real> forwardVector = GetForwardVector();
  const Vector3<Real> upVector      = GetUpVector();
  const Vector3<Real> lookAtPoint   = position + forwardVector;
  gluLookAt(position.x,    position.y,    position.z,
            lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
            upVector.x,    upVector.y,    upVector.z);
}

#endif
