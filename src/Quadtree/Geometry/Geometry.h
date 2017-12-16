/*******************************************************************************
* File:           Geometry.h
* Author:         Christian Alfons
* Date created:   2010-07-16
* Date modified:  2010-09-23
* Description:    Geometry base class.
*******************************************************************************/

#ifndef GEOMETRY_H_INCLUDED
#define GEOMETRY_H_INCLUDED

#include "../Math/VectorMath.h"

class Geometry
{
  protected:
    // Instance variables
    Vector3<double>   position;
    Matrix3x3<double> orientation;

  public:
    // Constructor
    Geometry(const Vector3<double> &position = Vector3<double>(0.0, 0.0, 0.0), const Matrix3x3<double> &orientation = Matrix3x3<double>(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0));

    // Instance methods
    Vector2<int>      GetScreenPosition(const bool flipY = true, const bool useOrigin = false) const;
    Vector3<double>   GetPosition() const;
    void              SetPosition(const Vector3<double> &position);
    Matrix3x3<double> GetOrientation() const;
    void              SetOrientation(const Matrix3x3<double> &orientation);
    Vector3<double>   TransformToLocal(const Vector3<double> &v, const bool translate = true) const;
    Vector3<double>   TransformToWorld(const Vector3<double> &v, const bool translate = true) const;
    virtual void      Render() const = 0;
};

#endif
