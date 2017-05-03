/*******************************************************************************
* File:           TextTool.h
* Author:         Christian Alfons
* Date created:   2010-07-05
* Date modified:  2010-09-11
* Description:    A nonflexible but simple and easy-to-use ANSI-extended ASCII
*                 text tool for OpenGL. Uses a custom retro "pixel font".
*******************************************************************************/

#ifndef TEXTTOOL_H_INCLUDED
#define TEXTTOOL_H_INCLUDED

#include <cstring>
#include <GL/gl.h>

class TextTool
{
  private:
    // Class variables
    static GLuint   instanceCount;
    static GLuint   texture;
    static GLuint   displayListBase;

    // Class methods
    static void     CreateFontTexture();
    static void     CreateDisplayLists();

    // Instance variables
    GLfloat         scale;

  public:
    // Constructor
    TextTool(const float scale = 1.0);

    // Destructor
    ~TextTool();

    // Instance methods
    void            RenderText(const char text[], const int x = 0, const int y = 0);
};

#endif
