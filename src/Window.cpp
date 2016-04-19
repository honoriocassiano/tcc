/*
 * Window.cpp
 *
 *  Created on: 14 de abr de 2016
 *      Author: cassiano
 */

#include "Window.h"

#include <GL/gl.h>
#include <GL/glut.h>

// ========================================================
// static variables of Window class

// State of the mouse cursor
int Window::mouseButton;
int Window::mouseX;
int Window::mouseY;

int Window::display_list_index;
ArgParser* Window::args;
Camera* Window::camera;
Mesh* Window::mesh;

Window * Window::mInstance;

// ========================================================

void Window::InitLight() {
	// Set the last component of the position to 0 to indicate
	// a directional light source

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
	float back_color[] = { 0.0, 0.0, 1.0, 1 };
	glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, back_color);
	glEnable(GL_LIGHT1);
}

void Window::display(void) {
	// Clear the display buffer, set it to the background color
	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set the camera parameters
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	InitLight(); // light will be a headlamp!
	camera->glPlaceCamera();

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	mInstance->update();

	HandleGLErrorWindow();

	// Swap the back buffer with the front buffer to display
	// the scene
	glutSwapBuffers();
}

// ========================================================
// Callback function for window resize
// ========================================================

void Window::reshape(int w, int h) {
	// Set the OpenGL viewport to fill the entire window
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);

	// Set the camera parameters to reflect the changes
	camera->glInit(w, h);

	args->width = w;
	args->height = h;
}

// ========================================================
// Callback function for mouse click or release
// ========================================================

void Window::mouse(int button, int state, int x, int y) {
	// Save the current state of the mouse.  This will be
	// used by the 'motion' function
	mouseButton = button;
	mouseX = x;
	mouseY = y;
}

// ========================================================
// Callback function for mouse drag
// ========================================================

void Window::motion(int x, int y) {
	// Left button = rotation
	// (rotate camera around the up and horizontal vectors)
	if (mouseButton == GLUT_LEFT_BUTTON) {
		camera->rotateCamera(0.005 * (mouseX - x), 0.005 * (mouseY - y));
		mouseX = x;
		mouseY = y;
	}
	// Middle button = translation
	// (move camera perpendicular to the direction vector)
	else if (mouseButton == GLUT_MIDDLE_BUTTON) {
		camera->truckCamera((mouseX - x) * 0.05, (y - mouseY) * 0.05);
		mouseX = x;
		mouseY = y;
	}
	// Right button = zoom
	// (move camera along the direction vector)
	else if (mouseButton == GLUT_RIGHT_BUTTON) {
		camera->dollyCamera((x - mouseX) * 0.05);
		mouseX = x;
		mouseY = y;
	}

	// Redraw the scene with the new camera parameters
	glutPostRedisplay();
}

// ========================================================
// Callback function for keyboard events
// ========================================================

void Window::keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
	case 'W':
		args->wireframe = !args->wireframe;
		Render();
		break;
	case 'g':
	case 'G':
		args->gouraud = !args->gouraud;
		Render();
		break;
	case 's':
	case 'S':
		mesh->LoopSubdivision();
		Render();
		break;
	case 'd':
	case 'D':
		mesh->Simplification((int) floor(0.9 * mesh->numTriangles()));
		Render();
		break;
	case 'q':
	case 'Q':
		exit(0);
		break;
	default:
		printf("UNKNOWN KEYBOARD INPUT  '%c'\n", key);
	}
}

// ========================================================
// Initialize all appropriate OpenGL variables, set
// callback functions, and start the main event loop.
// This function will not return but can be terminated
// by calling 'exit(0)'
// ========================================================

void Window::initialize(ArgParser *_args, Mesh *_mesh) {

	args = _args;
	mesh = _mesh;
	camera = new PerspectiveCamera(Vec3f(0, 0, 5), Vec3f(0, 0, -1),
			Vec3f(0, 1, 0), 20 * M_PI / 180.0);

	// Set global lighting parameters
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);

	// Set window parameters
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glEnable(GL_DEPTH_TEST);
	glutInitWindowSize(args->width, args->height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("TCC");

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_NORMALIZE);

	// Ambient light
	Vec3f ambColor = Vec3f(0.2, 0.2, 0.2);
	GLfloat ambArr[] = { ambColor.x(), ambColor.y(), ambColor.z(), 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambArr);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);

	display_list_index = glGenLists(1);

	// Initialize callback functions
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	mClock.start();

	Render();

	// Enter the main rendering loop
	glutMainLoop();
}

void Window::update() {
	Vec3f center = mBodies[0]->getCenter();

	//glLoadIdentity();
	//glTranslatef(center.x(), center.y(), center.z());
	glPushMatrix();
	//glTranslatef(0.1f, 0.1f, 0.1f);
	glTranslatef(center.x(), center.y(), center.z());

	glCallList(display_list_index);

	glPopMatrix();
}

void Window::idle(void) {
//	const Vec3f speed = Vec3f(.1f, .1f, .1f);
//
//	mInstance->mBodies[0]->setCenter(
//			mInstance->mBodies[0]->getCenter()
//					+ (speed * mInstance->mClock.getTime().getAsSeconds()));
//
//	mInstance->mClock.restart();
//
//	glutPostRedisplay();
}

void Window::Render() {
	glNewList(display_list_index, GL_COMPILE_AND_EXECUTE);
	// =========================================================
	// put your GL drawing calls inside the display list for efficiency
	//for (auto body : mInstance->mBodies) {
	//mesh->Paint(args);
	mInstance->mBodies[0]->getMesh()->Paint(args);
	//}

	// =========================================================
	glEndList();
	glutPostRedisplay();
}

// ========================================================
// ========================================================

int HandleGLErrorWindow() {
	GLenum error;
	int i = 0;
	while ((error = glGetError()) != GL_NO_ERROR) {
		printf("GL ERROR(%d):  %s\n", i, gluErrorString(error));
		i++;
	}
	if (i == 0)
		return 1;
	return 0;
}

