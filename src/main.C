#include <stdio.h>

#include "argparser.h"

#include <GL/glut.h>
#include "Planet.h"

#include "SDLWindow.h"

// =========================================
// =========================================

int main(int argc, char **argv) {
	srand(time(NULL));

	SDLWindow window(640, 480);

	Planet* planet1 = new Planet(5);
	planet1->setCenter(Vec3f(50, 0, 0));
	planet1->setMass(5);

	Planet* planet2 = new Planet(5);
	planet2->setCenter(Vec3f(100, 0, 0));
	planet2->setMass(5);

	Planet* planet3 = new Planet(5);
	planet3->setCenter(Vec3f(200, 0, 0));
	planet3->setMass(5);

	Planet* sun = new Planet(10);
	sun->setCenter(Vec3f(0, 0, 0));
	sun->setMass(10000);

	planet1->setOrbiter(sun);
	planet2->setOrbiter(sun);
	planet3->setOrbiter(sun);

	window.addBody(planet1);
	window.addBody(planet2);
	window.addBody(planet3);
	window.addBody(sun);

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
