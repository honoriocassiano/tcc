#include "Debug.h"
#include <assert.h>

//#include <GL/glut.h>
#include "structures/vectors.h"
//#include "Planet.h"

#include "SDLWindow.h"

#include "MultiLevelArray.h"

#include "Greeting.h"

#include "SDLApplication.h"

// =========================================
// =========================================

int main(int argc, char **argv) {

	sayHello();

	SDLApplication app(800, 600);

	app.Run();

	Log("Finished!\n");

	return 0;
}
