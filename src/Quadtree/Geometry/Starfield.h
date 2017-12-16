/*******************************************************************************
* File:           Starfield.h
* Author:         Christian Alfons
* Date created:   2010-09-07
* Date modified:  2011-01-23
* Description:    Generates and renders simple particle starfields, using
*                 pre-billboarded stars (assuming the camera doesn't move
*                 relative to the stars).
*******************************************************************************/

#ifndef STARFIELD_H_INCLUDED
#define STARFIELD_H_INCLUDED

#include "Star.h"

class Starfield
{
  private:
    // Class variables
    static unsigned int instanceCount;
    static GLuint texture;

    // Instance variables
    GLuint  vbo, ibo;
    GLsizei elementCount;

  public:
    // Constructor
    Starfield(const unsigned int numberOfStars = 1024, const float starSizeMin = 1.0, const float starSizeMax = 3.0);

    // Destructor
    ~Starfield();

    // Instance methods
    void Render() const;
};

#endif
