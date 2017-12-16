/*******************************************************************************
* File:           SimpleCircle.h
* Author:         Christian Alfons
* Date created:   2011-02-16
* Date modified:  2011-02-16
* Description:    A simple OpenGL circle class.
*******************************************************************************/

#ifndef SIMPLECIRCLE_H_INCLUDED
#define SIMPLECIRCLE_H_INCLUDED

#include "../GL/GLee.h"

class SimpleCircle
{
  private:
    // Instance variables
    GLuint  vbo, ibo;
    GLsizei elementCount;

  public:
    // Constructors
    SimpleCircle(const float radius, const unsigned int points = 32);

    // Destructor
    ~SimpleCircle();

    // Instance methods
    void Render() const;
};

#endif
