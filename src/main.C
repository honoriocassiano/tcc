#include "Debug.h"
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
