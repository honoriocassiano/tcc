/*******************************************************************************
* File:           Star.h
* Author:         Christian Alfons
* Date created:   2011-02-10
* Date modified:  2011-02-10
* Description:    Billboard star class.
*******************************************************************************/

#ifndef STAR_H_INCLUDED
#define STAR_H_INCLUDED

#include "AstronomicalObject.h"
#include "../Shaders/ShaderManager.h"

class Star : public AstronomicalObject
{
  private:
    // Class variables
    static unsigned int instanceCount;
    static GLuint texture;

    // Instance variables
    const double radius;

  public:
    // Class methods
    static GLuint GenerateStarTexture(const unsigned int size);

    // Constructor
    Star(const double radius);

    // Destructor
    virtual ~Star();

    // Instance methods
    virtual double          GetBoundingRadius() const;
    virtual double          GetHoverBoundingRadius() const;
    virtual Vector3<double> GetClosestSurfacePoint(const Vector3<double> &pos) const;
    virtual double          GetClosestSurfaceDistance(const Vector3<double> &pos) const;
    void                    Render() const;
};

#endif
