/*******************************************************************************
* File:           ShaderManager.h
* Author:         Christian Alfons
* Date created:   2009-10-19
* Date modified:  2011-01-17
* Description:    Helper class for organizing GLSL shader programs.
*******************************************************************************/

#ifndef SHADERMANAGER_H_INCLUDED
#define SHADERMANAGER_H_INCLUDED

#include "../GL/GLee.h"
#include <GL/glfw.h>
#include <string>
#include "GLSL/ShaderSources.h"

using std::string;

class ShaderManager
{
  private:
    static bool   initialized;
    static GLuint programs[NONE];

    static void ReportErrors(const GLuint handle, const bool isProgram = false);

  public:
    static void   Initialize();
    static void   Deinitialize();
    static GLuint GetShaderProgramHandle(const unsigned int program);
    static void   UseShaderProgram(const unsigned int program);
};

#endif
