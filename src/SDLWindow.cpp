/*
 * SDLWindow.cpp
 *
 *  Created on: 28 de abr de 2016
 *      Author: cassiano
 */

#include "SDLWindow.h"

#include <algorithm>
#include <assert.h>

#include "Debug.h"

//******************************************************
#include "Quadtree.h"
#include "MeshDrawer.h"

Quadtree* tree = new Quadtree(Vec3f(0, 0, 0), Vec3f(1, 0, 0), Vec3f(0, 1, 0), 0,
		20);

QuadCube* cube = new QuadCube(Vec3f(0, 0, 0));
//Quadtree* tree = new Quadtree(Vec3f(0, 0, 0), Vec3f(50, 0, -10), Vec3f(0, 50, -10), 0, 20);

Quadtree2* tree2 = nullptr;

Quadtree2** getTree2() {
	return &tree2;
}

Quadtree* getTree() {
	return tree;
}

QuadCube* getCube() {
	return cube;
}
//******************************************************

//******************************************************
//Landscape* landscape = new Landscape(0.5f);
//Vec3f position(0.5, 0.5, 10);
//Vec3f position(2.5, 2.5, 100);
//const Vec3f position(100, 0, 40);
const Vec3f position(6.2, 1.2, -28.8);
int globalI = 0;

/*
 Landscape* getLandscape() {
 return landscape;
 }
 */
//******************************************************
int HandleGLErrorWindow2() {
	GLenum error;
	int i = 0;
	while ((error = glGetError()) != GL_NO_ERROR) {
		Error("Not rectangle!\n");
		i++;
	}
	if (i == 0)
		return 1;
	return 0;
}

SDLWindow::SDLWindow(int width, int height) :
		mCamera(nullptr), mClock(new Clock()), mIsRunning(false), mWindow(
				nullptr), mWidth(width), mHeight(height) {

//	options.normals = true;
//	options.wireframe = true;

//	Vec3f position(0, 1500, 100);
//	Vec3f direction = Vec3f(0, 0, 0) - position;
	Vec3f direction = Vec3f(2.5, 2.5, 0) - position;

	direction.Normalize();

	mCamera = new PerspectiveCamera2(position, direction, Vec3f(0, 1, 0),
			20 * M_PI / 180.0);

	assert(initSDL());
	assert(initOpenGL());

	mCamera->glInit(mWidth, mHeight);
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
//	while (mIsRunning && (globalI < 10)) {
		//******************************************************
		SDL_Event e;

		processRealtimeEvents();

		while (SDL_PollEvent(&e)) {
			processEvents(e);
		}

		Time dt = mClock->getTime();

		update(dt);

		mClock->restart();

		display();

		//******************************************************
		//const Patch* p = landscape->getPatch();
		/*
		 Patch* p = landscape->getPatch();

		 Log("%2d - vertices: %d, triangles: %d\n", globalI,
		 p->getMesh()->numVertices(), p->getMesh()->numTriangles());
		 */

		++globalI;
		//******************************************************
	}

	close();
}

void SDLWindow::update(const Time& dt) {
	//******************************************************
	/*
	 landscape->computeVariance();
	 //landscape->tessellate(position);
	 landscape->tessellate(mCamera->getPosition());
	 //	for (CelestialBody* body : mBodies) {
	 //		body->update(dt);
	 //	}
	 */
	//******************************************************

//	(*getTree2())->update2(mCamera->getPosition());
	getCube()->update(mCamera->getPosition());

//	MeshDrawer::draw(tree2->getMesh(), options);
}

void SDLWindow::processRealtimeEvents() {
	const Uint8 *state = SDL_GetKeyboardState(nullptr);

	if (state[SDL_SCANCODE_UP]) {
		mCamera->dollyCamera(0.5f);
//		mCamera->dollyCamera(2.0f);
	} else if (state[SDL_SCANCODE_DOWN]) {
		mCamera->dollyCamera(-0.5f);
//		mCamera->dollyCamera(-2.0f);
	}

	if (state[SDL_SCANCODE_LEFT]) {
		mCamera->truckCamera(-0.025, 0);
	} else if (state[SDL_SCANCODE_RIGHT]) {
		mCamera->truckCamera(0.025, 0);
	}

	int x = 0, y = 0;

	// A posição em y deve ser invertida por causa das coordenadas do SDL
	Uint32 mouseState = SDL_GetRelativeMouseState(&x, &y);

	if ((x != 0 || y != 0) && mouseState) {
		if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
			mCamera->rotateCamera(-0.005 * x, -0.005 * y);
			//mCamera->rotateCamera(-0.005 * x, -0.005 * y);

//			Log("%s", mCamera->getPosition().str().c_str());
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
	// Place amera
	mCamera->glPlaceCamera();

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	HandleGLErrorWindow2();

	//******************************************************
	//	for (CelestialBody* body : mBodies) {
	//		body->draw();
	//	}
//	MeshDrawer::draw(tree->getMesh(), true, false);

//	cube->draw(true, false);
	//cube->draw(false, false);
//	cube->draw(options);
	MeshDrawer::draw(tree2->getMesh(), options);

//	landscape->render();
	//******************************************************

	// Trocar buffers
	SDL_GL_SwapWindow(mWindow);
}

bool SDLWindow::initSDL() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		Error("Error initializing SDL: %s\n", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow("TCC", SDL_WINDOWPOS_UNDEFINED,
	SDL_WINDOWPOS_UNDEFINED, mWidth, mHeight, SDL_WINDOW_OPENGL);

	if (!mWindow) {
		Error("Error creating window: %s\n", SDL_GetError());
		return false;
	}

	SDL_GLContext mGLContext = SDL_GL_CreateContext(mWindow);

	return true;
}

void SDLWindow::close() {
	SDL_GL_DeleteContext(mGLContext);

	SDL_Quit();
}

bool SDLWindow::initOpenGL() {
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);

//		glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glEnable(GL_DEPTH_TEST);
//		glutInitWindowSize(args->width, args->height);
//	glutInitWindowPosition(100, 100);
//	glutCreateWindow("TCC");

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_NORMALIZE);

	// Ambient light
	Vec3f ambColor = Vec3f(0.2, 0.2, 0.2);
	GLfloat ambArr[] = { ambColor.x(), ambColor.y(), ambColor.z(), 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambArr);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glCullFace(GL_BACK);

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
			(*getTree2())->getMesh()->printTriangles();
			Log("Triangles: %d", (*getTree2())->getMesh()->numTriangles());
			break;
		}

		case SDLK_v: {
			(*getTree2())->getMesh()->printVertices();
			Log("Vertices: %d", (*getTree2())->getMesh()->numVertices());
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
