/*
 * SDLWindow.h
 *
 *  Created on: 28 de abr de 2016
 *      Author: cassiano
 */

#ifndef SRC_SDLWINDOW_H_
#define SRC_SDLWINDOW_H_

#include <vector>

#include "camera.h"
#include "Time.h"
#include "Clock.h"
#include "CelestialBody.h"

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "PerspectiveCamera2.h"
#include "QuadCube.h"
#include "MeshDrawer.h"
#include "Quadtree.h"

#include "World.h"

class SDLWindow {
public:
	SDLWindow(int width, int height);
	virtual ~SDLWindow();

	void addBody(CelestialBody * body);
	bool removeBody(CelestialBody * body);

	void run();

private:

	bool initSDL();
	bool initOpenGL();

	void configureLight();

	void close();

	void update(const Time& dt);
	void display();
	void processRealtimeEvents();
	void processEvents(const SDL_Event& e);

	QuadCube* cube;

	World* world;
	Vec3f lastPosition;

	std::vector<CelestialBody*> mBodies;
	std::vector<int> mIndices;

	DrawOptions options;
	SDL_Window * mWindow;

	PerspectiveCamera2* mCamera;
	Clock * mClock;
	bool freezed;
	bool mIsRunning;
	SDL_GLContext mGLContext;

	int mWidth;
	int mHeight;
};

#endif /* SRC_SDLWINDOW_H_ */
