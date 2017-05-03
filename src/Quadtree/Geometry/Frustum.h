/*******************************************************************************
* File:           Frustum.h
* Author:         Christian Alfons
* Date created:   2010-07-08
* Date modified:  2010-09-20
* Description:    A simple class for efficient OpenGL view frustum culling.
*******************************************************************************/

#ifndef FRUSTUM_H_INCLUDED
#define FRUSTUM_H_INCLUDED

#include <vector>
#include "../Math/VectorMath.h"

#define OUTSIDE       0
#define INSIDE        1
#define INTERSECTING  2

using std::vector;

class Frustum
{
  private:
    // Instance variables
    Vector4<double> planes[6];
    double CalculateDistanceToPlane(const int plane, const Vector3<double> &point) const;
    void NormalizePlanes();

  public:
    // Constructor
    Frustum();

    // Instance methods
    bool  ContainsPoint(const Vector3<double> &point) const;
    int   ContainsSphere(const Vector3<double> &position, const double radius) const;
    int   ContainsBox(const Vector3<double> &min, const Vector3<double> &max) const;
    int   ContainsPolygon(const vector<Vector3<double> > &points) const;
};

#endif
