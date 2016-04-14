#include <stdio.h>

#include "argparser.h"
#include "glCanvas.h"

#include "Sphere.hpp"

#include <GL/glut.h>

// =========================================
// =========================================

int main(int argc, char *argv[]) {
	//srand48(0);
	srand(time(NULL));

	glutInit(&argc, argv);

	ArgParser *args = new ArgParser(argc, argv);

	Sphere* sphere = new Sphere(30, 100, 100);

	// mesh->Load(args->input_file);

	// mesh->LoopSubdivision();

	// sphere->setCenter(Vec3f(5, -1, 2));

	GLCanvas glcanvas;
	glcanvas.initialize(args, sphere->getMesh());

	//delete args;

	return 0;
}

// =========================================
// =========================================
