/*
 * SDLApplication.cpp
 *
 *  Created on: 17 de mai de 2017
 *      Author: cassiano
 */

#include "SDLApplication.h"

SDLApplication::SDLApplication(int _width, int _height) :
		width(_width), height(_height), isRunning(false) {

	InitSDL();

}

void SDLApplication::ResetScene() {
}

void SDLApplication::UsePerspectiveProjection(const double zNear,
		const double zFar) const {
}

void SDLApplication::UsePerspectiveProjection(
		const AstronomicalObject* astronomicalObject) const {
}

void SDLApplication::UseOrthogonalProjection() const {
}

void SDLApplication::RestoreProjection() const {
}

void SDLApplication::Move(const double frameTime) {
}

double SDLApplication::GetDistanceToClosestAstronomicalObject() const {
}

void SDLApplication::ProcessRealtimeEvents() {
}

void SDLApplication::ProcessEvent(SDL_Event& event) {

}

bool SDLApplication::InitSDL() {

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Error initializing SDL: %s\n", SDL_GetError());
		return false;
	}

	window = SDL_CreateWindow("TCC", SDL_WINDOWPOS_UNDEFINED,
	SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);

	if (!window) {
		printf("Error creating window: %s\n", SDL_GetError());
		return false;
	}

	glContext = SDL_GL_CreateContext(window);

	return true;
}

SDLApplication::~SDLApplication() {
	// TODO Auto-generated destructor stub
}

void SDLApplication::Update(const Time& dt) {

	const double frameTime = dt.getAsSeconds();

	// Update particle system
	Particle2D::UpdateAllParticles(frameTime);

	// Sort astronomical objects, closest to camera first
	astronomicalObjectComparer.cameraPosition = camera->GetPosition();
	sort(astronomicalObjects.begin(), astronomicalObjects.end(),
			astronomicalObjectComparer);

	// Update frame rate and move the camera
	fpsCounter->Update(frameTime);
	Move(frameTime);

	// Update the scene
	hoveredAstronomicalObjectIndex = -1;
	for (unsigned int i = 0; i < astronomicalObjects.size(); i++) {
		// Set the projection so the view frustum is built correctly
		UsePerspectiveProjection(astronomicalObjects[i]);

		// If a hovered object has already been found, it is the closest one (since they are sorted)
		if (hoveredAstronomicalObjectIndex == -1) {
			// Get matrices and viewport
			GLdouble modelview[16], projection[16];
			GLint viewport[4];
			glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
			glGetDoublev(GL_PROJECTION_MATRIX, projection);
			glGetIntegerv(GL_VIEWPORT, viewport);

			// Perform picking
			Vector2<int> mousePosInt;
			glfwGetMousePos(&mousePosInt.x, &mousePosInt.y);
			const Vector2<double> mousePos(mousePosInt.x,
					viewport[3] - mousePosInt.y);
			Vector3<double> rayStart, rayEnd;
			gluUnProject((double) mousePos.x, (double) mousePos.y, 0.0,
					modelview, projection, viewport, &rayStart.x, &rayStart.y,
					&rayStart.z);
			gluUnProject((double) mousePos.x, (double) mousePos.y, 1.0,
					modelview, projection, viewport, &rayEnd.x, &rayEnd.y,
					&rayEnd.z);
			const Vector3<double> rayDir = (rayEnd - rayStart).GetNormalized();

			// If hovered and in front of camera (either intersection > 0)
			double t0, t1;
			if (GetLineSphereIntersection(rayStart, rayDir,
					astronomicalObjects[i]->GetHoverBoundingRadius(), t0, t1)
					&& (t0 > 0.0 || t1 > 0.0))
				hoveredAstronomicalObjectIndex = i;
		}

		// If the object is a planet
		if (Planet *planet = dynamic_cast<Planet*>(astronomicalObjects[i])) {
			planet->Animate(
					timeUnitSeconds[animationTimeUnit]
							* (reverseTime ? -frameTime : frameTime));
			planet->SetCameraPosition(camera->GetPosition());

			// If not frozen; update
			if (!freezeLod) {
				planet->UpdateFrustum();
				planet->Update();
			}
		}

		RestoreProjection();
	}
}

bool SDLApplication::InitGL() {

	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);

//	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
//	glEnable(GL_NORMALIZE);
//
//	// Ambient light
//	Vec3f ambColor = Vec3f(0.2, 0.2, 0.2);
//	GLfloat ambArr[] = { ambColor.x(), ambColor.y(), ambColor.z(), 1.0 };
//	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambArr);
//
//	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
//	glCullFace(GL_BACK);

	return true;
}

void SDLApplication::Run() {

	isRunning = true;

	clock.start();

	while (isRunning) {
		ProcessRealtimeEvents();

		SDL_Event e;

		while (SDL_PollEvent(&e)) {
			ProcessEvent(e);
		}

		Time dt = clock.getTime();

		Update(dt);

		clock.restart();
	}

}

void SDLApplication::Render() const {
}
