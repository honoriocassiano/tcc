///*
// * SDLWindow.h
// *
// *  Created on: 28 de abr de 2016
// *      Author: cassiano
// */
//
//#ifndef SRC_SDLWINDOW_H_
//#define SRC_SDLWINDOW_H_
//
//#include <vector>
//
//#include "structures/camera.h"
//#include "Time.h"
//#include "Clock.h"
////#include "CelestialBody.h"
//
//#include <SDL2/SDL.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
//#include "PerspectiveCamera.h"
//#include "Quadtree/Application/Application.h";
//
//#include "MeshDrawer.h"
//
//class SDLWindow {
//public:
//	SDLWindow(int width, int height);
//	virtual ~SDLWindow();
//
////	void addBody(CelestialBody * body);
////	bool removeBody(CelestialBody * body);
//
//	void run();
//
//private:
//
//	bool initSDL();
//	bool initOpenGL();
//
//	void configureLight();
//
//	void close();
//
//	void update(const Time& dt);
//	void display();
//	void processRealtimeEvents();
//	void processEvents(const SDL_Event& e);
//
//	Vec3f lastPosition;
//
//	std::vector<CelestialBody*> mBodies;
//	std::vector<int> mIndices;
//
//	DrawOptions options;
//	SDL_Window * mWindow;
//
//	Application* application;
//
//	PerspectiveCamera* mCamera;
////	Clock * mClock;
//	bool freezed;
//	bool mIsRunning;
//	SDL_GLContext mGLContext;
//
//	int mWidth;
//	int mHeight;
//};
//
//#endif /* SRC_SDLWINDOW_H_ */
