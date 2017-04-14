/*
 * SDLWindow.cpp
 *
 *  Created on: 28 de abr de 2016
 *      Author: cassiano
 */

#include "SDLWindow.h"

#include <algorithm>
#include <assert.h>

#include "WorldMesh.h"
#include "MeshDrawer.h"

//const Vec3f position(6.2, 1.2, -28.8);

int HandleGLErrorWindow2() {
	GLenum error;
	int i = 0;
	while ((error = glGetError()) != GL_NO_ERROR) {
		fprintf(stderr, "%u\n", error);
		i++;
	}
	if (i == 0)
		return 1;
	return 0;
}

SDLWindow::SDLWindow(int width, int height) :
		mCamera(nullptr), mClock(new Clock()), freezed(false), mIsRunning(
				false), mWindow(nullptr), mWidth(width), mHeight(height) {

//	options.normals = true;
//	options.wireframe = true;

	const Vec3f position(1000, 500, 0);

	Vec3f direction = (Vec3f(0, 0, 0) - position).normalized();

	mCamera = new PerspectiveCamera(position, direction, Vec3f(0, 1, 0),
			20 * M_PI / 180.0);

	assert(initSDL());
	assert(initOpenGL());

	mCamera->glInit(mWidth, mHeight);

	lastPosition = mCamera->getPosition();
}

SDLWindow::~SDLWindow() {
	SDL_DestroyWindow(mWindow);

	delete mCamera;
	delete mClock;
}

void SDLWindow::addBody(CelestialBody * body) {
	mBodies.push_back(body);
}

bool SDLWindow::removeBody(CelestialBody * body) {
	auto it = std::find(mBodies.begin(), mBodies.end(), body);

	if (it != mBodies.end()) {
		mBodies.erase(it);
		return true;
	}
	return false;
}

void SDLWindow::run() {
	mIsRunning = true;

	mClock->start();

	//******************************************************
	while (mIsRunning) {
		SDL_Event e;

		processRealtimeEvents();

		while (SDL_PollEvent(&e)) {
			processEvents(e);
		}

		Time dt = mClock->getTime();

		if (!freezed) {
			update(dt);
		}

		mClock->restart();

		display();

	}

	close();
}

void SDLWindow::update(const Time& dt) {
//	if (lastPosition != mCamera->getPosition()) {

//	worldMesh.rotate(M_PI / 360);
//
	lastPosition = mCamera->getPosition();
//
//	worldMesh.update(lastPosition);

	for (auto& b : mBodies) {
		b->updateLOD(lastPosition);
		b->update(dt);
	}

//	}
}

void SDLWindow::processRealtimeEvents() {
	const Uint8 *state = SDL_GetKeyboardState(nullptr);

	float dolly = 0;
	float truck = 0;

	if (state[SDL_SCANCODE_UP]) {
		dolly = 0.5f;
	} else if (state[SDL_SCANCODE_DOWN]) {
		dolly = -0.5f;
	}

	if (state[SDL_SCANCODE_LEFT]) {
		truck = -0.5;
	} else if (state[SDL_SCANCODE_RIGHT]) {
		truck = 0.5;
	}

	if(state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT] ) {
		dolly *= 10;
		truck *= 10;
	}

	mCamera->dollyCamera(dolly);
	mCamera->truckCamera(truck, 0);

	int x = 0, y = 0;

	// A posição em y deve ser invertida por causa das coordenadas do SDL
	Uint32 mouseState = SDL_GetRelativeMouseState(&x, &y);

	if ((x != 0 || y != 0) && mouseState) {
		if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
			mCamera->rotateCamera(-0.005 * x, -0.005 * y);
			//mCamera->rotateCamera(-0.005 * x, -0.005 * y);
		}
		// Inserir ações com outros botões aqui
	}
}

void SDLWindow::display() {
//	glClearColor(1, 1, 1, 0);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// init light
	//InitLight(); // light will be a headlamp!
	configureLight();
	// Place xamera
	mCamera->glPlaceCamera();

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	HandleGLErrorWindow2();

	for (CelestialBody* body : mBodies) {
		MeshDrawer::draw(body, options);
	}

//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

//	MeshDrawer::draw(&worldMesh, options);

	// Trocar buffers
	SDL_GL_SwapWindow(mWindow);
}

bool SDLWindow::initSDL() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Error initializing SDL: %s\n", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow("TCC", SDL_WINDOWPOS_UNDEFINED,
	SDL_WINDOWPOS_UNDEFINED, mWidth, mHeight, SDL_WINDOW_OPENGL);

	if (!mWindow) {
		printf("Error creating window: %s\n", SDL_GetError());
		return false;
	}

	mGLContext = SDL_GL_CreateContext(mWindow);

	return true;
}

void SDLWindow::close() {
	SDL_GL_DeleteContext(mGLContext);

	SDL_Quit();
}

bool SDLWindow::initOpenGL() {
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_NORMALIZE);

	// Ambient light
	Vec3f ambColor = Vec3f(0.2, 0.2, 0.2);
	GLfloat ambArr[] = { ambColor.x(), ambColor.y(), ambColor.z(), 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambArr);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
//	glCullFace(GL_BACK);

	return true;
}

void SDLWindow::configureLight() {
	GLfloat position[4] = { 30, 30, 100, 1 };
	GLfloat diffuse[4] = { 0.75, 0.75, 0.75, 1 };
	GLfloat specular[4] = { 0, 0, 0, 1 };
	GLfloat ambient[4] = { 0.2, 0.2, 0.2, 1.0 };

	GLfloat zero[4] = { 0, 0, 0, 0 };
	glLightfv(GL_LIGHT1, GL_POSITION, position);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT1, GL_AMBIENT, zero);
	glEnable(GL_LIGHT1);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_COLOR_MATERIAL);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

	GLfloat spec_mat[4] = { 1, 1, 1, 1 };
	float glexponent = 30;
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec_mat);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
//	float back_color[] = { 0.0, 0.0, 1.0, 1 };
//	glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, back_color);
	glColorMaterial(GL_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_LIGHT1);
}

void SDLWindow::processEvents(const SDL_Event& e) {
	if (e.type == SDL_WINDOWEVENT) {
		if (e.window.event == SDL_WINDOWEVENT_CLOSE) {
			mIsRunning = false;
		}
	}

	if (e.type == SDL_KEYUP) {
		switch (e.key.keysym.sym) {

		case SDLK_c: {
			mCamera->lookAt(Vec3f(0, 0, 0));
			break;
		}

		case SDLK_f: {
			freezed = !freezed;
			break;
		}

		case SDLK_n: {
			options.normals = !options.normals;
			break;
		}

		case SDLK_o: {
			options.noise = !options.noise;
			break;
		}

		case SDLK_p: {
			options.points = !options.points;
			break;
		}

		case SDLK_t: {
//			cube->getMesh()->printTriangles();
//			Log("Triangles: %d", (*getTree2())->getMesh()->numTriangles());
			break;
		}

		case SDLK_v: {
//			cube->getMesh()->printVertices();
//			Log("Vertices: %d", (*getTree2())->getMesh()->numVertices());
			break;
		}

		case SDLK_k: {
//			cube->getMesh()->printTrianglesPointers();
//			Log("Vertices: %d", (*getTree2())->getMesh()->numVertices());
			break;
		}

		case SDLK_a: {
			options.axis = !options.axis;
			break;
		}

		case SDLK_h: {
			options.halfEdge = !options.halfEdge;
			break;
		}

		case SDLK_w: {
			options.wireframe = !options.wireframe;
			break;
		}

		case SDLK_ESCAPE: {
			mIsRunning = false;
			break;
		}

		default:
			break;
		}
	}
}
