/*******************************************************************************
* File:           Atmosphere.h
* Author:         Christian Alfons
* Date created:   2011-01-24
* Date modified:  2011-01-24
* Description:    Atmosphere class. Renders a basic sphere using an atmosphere
*                 shader program.
*******************************************************************************/

#ifndef ATMOSPHERE_H_INCLUDED
#define ATMOSPHERE_H_INCLUDED

#include "SimpleSphere.h"
#include "../Math/VectorMath.h"

class Planet;

class Atmosphere
{
  private:
    // Class variables
    static unsigned int   instanceCount;
    static SimpleSphere   *sphere;

    // Instance variables
    const Planet          *planet;
    const double          radius;
    const Vector3<float>  color;

  public:
    // Constructor
    Atmosphere(const Planet *planet, const double radius, const Vector3<float> &color);

    // Destructor
    ~Atmosphere();

    // Instance methods
    Vector3<float>  GetColor() const;
    double          GetRadius() const;
    void            Render() const;
};

#endif
