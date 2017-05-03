/*******************************************************************************
* File:           SimpleSphere.h
* Author:         Christian Alfons
* Date created:   2011-01-20
* Date modified:  2011-01-21
* Description:    A simple OpenGL sphere class.
*******************************************************************************/

#ifndef SIMPLESPHERE_H_INCLUDED
#define SIMPLESPHERE_H_INCLUDED

#include "../GL/GLee.h"

class SimpleSphere
{
  private:
    // Instance variables
    GLuint  vbo, ibo;
    GLsizei elementCount;

  public:
    // Constructors
    SimpleSphere(const float radius, const unsigned int lonSegs, const unsigned int latSegs);

    // Destructor
    ~SimpleSphere();

    // Instance methods
    void Render() const;
};

#endif
