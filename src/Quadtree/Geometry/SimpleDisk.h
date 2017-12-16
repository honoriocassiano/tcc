/*******************************************************************************
* File:           SimpleDisk.h
* Author:         Christian Alfons
* Date created:   2011-02-13
* Date modified:  2011-02-13
* Description:    A simple OpenGL disk class.
*******************************************************************************/

#ifndef SIMPLEDISK_H_INCLUDED
#define SIMPLEDISK_H_INCLUDED

#include "../GL/GLee.h"

class SimpleDisk
{
  private:
    // Instance variables
    GLuint  vbo, ibo;
    GLsizei elementCount;

  public:
    // Constructors
    SimpleDisk(const float innerRadius, const float outerRadius, const unsigned int slices, const unsigned int loops = 1);

    // Destructor
    ~SimpleDisk();

    // Instance methods
    void Render() const;
};

#endif
