#include "Debug.h"
#include "Greeting.h"
#include "SDLApplication.h"


#include <GL/glut.h>



// =========================================
// =========================================

int main(int argc, char **argv) {

	sayHello();

	glutInit(&argc, argv);

	SDLApplication app(800, 600);

	app.Run();

	Log("Finished!\n");

	return 0;
}
