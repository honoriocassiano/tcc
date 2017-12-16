/*******************************************************************************
* File:           SimpleCylinder.h
* Author:         Christian Alfons
* Date created:   2011-02-13
* Date modified:  2011-02-13
* Description:    A simple OpenGL cylinder class.
*******************************************************************************/

#ifndef SIMPLECYLINDER_H_INCLUDED
#define SIMPLECYLINDER_H_INCLUDED

#include "../GL/GLee.h"

class SimpleCylinder
{
  private:
    // Instance variables
    GLuint  vbo, ibo;
    GLsizei elementCount;

  public:
    // Constructors
    SimpleCylinder(const float radiusBase, const float radiusTop, const float height, const unsigned int slices, const unsigned int stacks = 1, const float y0 = 0.0);

    // Destructor
    ~SimpleCylinder();

    // Instance methods
    void Render() const;
};

#endif
