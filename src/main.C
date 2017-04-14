#include "Debug.h"
#include <assert.h>

#include <GL/glut.h>
#include "structures/vectors.h"
#include "Planet.h"

#include "SDLWindow.h"

#include "MultiLevelArray.h"

#include "Greeting.h"

// =========================================
// =========================================

int main(int argc, char **argv) {

	sayHello();

//	SDLWindow window(800, 600);
//	window.run();

	SDLWindow window(800, 600);

	Planet* planet1 = new Planet(Vec3f(50, 0, 0), 5);
	planet1->setMass(5);

	Planet* planet2 = new Planet(Vec3f(100, 0, 0), 5);
	planet2->setMass(5);

	Planet* planet3 = new Planet(Vec3f(200, 0, 0), 5);
	planet3->setMass(5);

	Planet* sun = new Planet(Vec3f(0, 0, 0), 20);
	sun->setMass(10000);

	planet1->setOrbiter(sun);
	planet2->setOrbiter(sun);
	planet3->setOrbiter(sun);

	window.addBody(planet1);
	window.addBody(planet2);
	window.addBody(planet3);
	window.addBody(sun);

	window.run();

	Log("Finished!\n");

	return 0;
}
