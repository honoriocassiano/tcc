/*******************************************************************************
* File:           CloudLayer.h
* Author:         Christian Alfons
* Date created:   2011-01-17
* Date modified:  2011-02-09
* Description:    Cloud layer class. Renders a basic sphere using a cloud
*                 shader program.
*******************************************************************************/

#ifndef CLOUDLAYER_H_INCLUDED
#define CLOUDLAYER_H_INCLUDED

#include "SimpleSphere.h"

class Planet;

class CloudLayer
{
  private:
    // Class variables
    static unsigned int   instanceCount;
    static SimpleSphere   *sphere;

    // Instance variables
    const Planet          *planet;
    const double          radius;
    GLuint                texture;

  public:
    // Constructor
    CloudLayer(const Planet *planet, const double radius, const double cloudCover, const double cloudSharpness);

    // Destructor
    ~CloudLayer();

    // Instance methods
    double  GetRadius() const;
    void    Render() const;
};

#endif
