/*
 * Window.h
 *
 *  Created on: 14 de abr de 2016
 *      Author: cassiano
 */

#ifndef SRC_WINDOW_H_
#define SRC_WINDOW_H_

#include <stdlib.h>

#include "argparser.h"
#include "camera.h"
#include "mesh.h"

#include "CelestialBody.h"
#include "Clock.h"

#include <vector>
#include <algorithm>

// ====================================================================

class Window {

private:
	Clock mClock;

	static Window * mInstance;

	// State of the mouse cursor
	static int mouseButton;
	static int mouseX;
	static int mouseY;

	void update();

	// Callback functions for mouse and keyboard events
	static void idle(void);
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

	std::vector<CelestialBody*> mBodies;

public:
	// Constructor and destructor
	Window(void) {
		mInstance = this;
	}
	~Window(void) {
		mInstance = nullptr;
	}

	// Set up the canvas and enter the rendering loop
	// Note that this function will not return but can be
	// terminated by calling 'exit(0)'

	void initialize(ArgParser *_args, Mesh *_mesh);
	static void Render();

	void addBody(CelestialBody * body) {
		mBodies.push_back(body);
	}

	bool removeBody(CelestialBody * body) {
		auto it = std::find(mBodies.begin(), mBodies.end(), body);

		if (it != mBodies.end()) {
			mBodies.erase(it);
			return true;
		}
		return false;
	}

};

int HandleGLErrorWindow();

#endif /* SRC_WINDOW_H_ */
