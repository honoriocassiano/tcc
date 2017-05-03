/*******************************************************************************
* File:           Skybox.h
* Author:         Christian Alfons
* Date created:   2011-01-23
* Date modified:  2011-01-23
* Description:    A basic OpenGL skybox class.
*******************************************************************************/

#ifndef SKYBOX_H_INCLUDED
#define SKYBOX_H_INCLUDED

#include "../Shaders/ShaderManager.h"

#define SKYBOX_SIDES 6

class Skybox
{
  private:
    // Instance variables
    GLuint textures[SKYBOX_SIDES];
    GLuint displayList;

  public:
    // Constructors
    Skybox();

    // Destructor
    ~Skybox();

    // Instance methods
    void Render() const;
};

#endif
