#include <stdio.h>

#include "argparser.h"
//#include "glCanvas.h"
#include "Window.h"

#include <GL/glut.h>
#include "Sphere.h"

// =========================================
// =========================================

int main(int argc, char *argv[]) {
	//srand48(0);
	srand(time(NULL));

	glutInit(&argc, argv);

	ArgParser *args = new ArgParser(argc, argv);

	Sphere* sphere = new Sphere(30, 150, 150);

	// mesh->Load(args->input_file);

	// mesh->LoopSubdivision();

	// sphere->setCenter(Vec3f(5, -1, 2));

	//GLCanvas glcanvas;
	Window window;
	window.initialize(args, sphere->getMesh());
	//glcanvas.initialize(args, sphere->getMesh());

	//delete args;

	return 0;
}

// =========================================
// =========================================
