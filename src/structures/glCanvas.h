// ====================================================================
// GLCanvas class by Rob Jagnow.
//
// The GLCanvas can be created with a call to the 'initialize' routine,

// ====================================================================

#ifndef _GL_CANVAS_H_
#define _GL_CANVAS_H_

#include <stdlib.h>

#include "argparser.h"
#include "camera.h"
#include "mesh.h"

// ====================================================================

class GLCanvas {

private:

  // State of the mouse cursor
  static int mouseButton;
  static int mouseX;
  static int mouseY;

  // Callback functions for mouse and keyboard events
  static void display(void);
  static void reshape(int w, int h);
  static void mouse(int button, int state, int x, int y);
  static void motion(int x, int y);
  static void keyboard(unsigned char key, int x, int y);

  static ArgParser *args;
  static Camera *camera;
  static Mesh *mesh;

  static void InitLight();

  static int display_list_index;

public:
  // Constructor and destructor
  GLCanvas(void) { }
 ~GLCanvas(void) { }

  // Set up the canvas and enter the rendering loop
  // Note that this function will not return but can be
  // terminated by calling 'exit(0)'

 void initialize(ArgParser *_args, Mesh *_mesh);
 static void Render();

};

int HandleGLError();

// ====================================================================

#endif
