/*******************************************************************************
* File:           PlanetRing.h
* Author:         Christian Alfons
* Date created:   2011-02-16
* Date modified:  2011-02-17
* Description:    Planet ring class. Renders a basic disk using a planet ring
*                 shader program.
*******************************************************************************/

#ifndef PLANETRING_H_INCLUDED
#define PLANETRING_H_INCLUDED

#include "SimpleDisk.h"
#include "../Math/VectorMath.h"

class Planet;

class PlanetRing
{
  private:
    // Instance variables
    const Planet  *planet;
    SimpleDisk    *disk;
    GLuint        texture;

  public:
    // Constructor
    PlanetRing(const Planet *planet, const double innerRadius, const double outerRadius, const Vector4<double> &color);

    // Destructor
    ~PlanetRing();

    // Instance methods
    void Render() const;
};

#endif
