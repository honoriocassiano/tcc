#include <stdio.h>

#include "argparser.h"
//#include "glCanvas.h"
#include "Window.h"

#include <GL/glut.h>
#include "Planet.h"

#include "SDLWindow.h"

// =========================================
// =========================================

int main(int argc, char **argv) {
	srand(time(NULL));

	SDLWindow window(640, 480);

	Planet* planet = new Planet(5, 150, 150);

	window.addBody(planet);

	window.run();

	return 0;
}


/*
int main(int argc, char *argv[]) {
	//srand48(0);
	srand(time(NULL));

	glutInit(&argc, argv);

	ArgParser *args = new ArgParser(argc, argv);

	Planet* sphere = new Planet(5, 150, 150);

	// mesh->Load(args->input_file);

	// mesh->LoopSubdivision();

	// sphere->setCenter(Vec3f(5, -1, 2));

	//GLCanvas glcanvas;
	Window window;

	window.addBody(sphere);

	window.initialize(args, sphere->getMesh());
	//glcanvas.initialize(args, sphere->getMesh());

	//delete args;

	return 0;
}
*/

// =========================================
// =========================================
