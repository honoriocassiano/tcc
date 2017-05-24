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

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluPerspective(50.0, (double) windowWidth / (double) windowHeight, zNear,
			zFar);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	camera->Use();

}

void SDLApplication::UsePerspectiveProjection(
		const AstronomicalObject* astronomicalObject) const {

	const double distance = astronomicalObject->GetClosestSurfaceDistance(
			camera->GetPosition());
	const double zNear = 0.4 * distance;
	const double zFar = distance
			+ 3.0 * astronomicalObject->GetBoundingRadius();
	UsePerspectiveProjection(zNear, zFar);

	// Instead of translating the planet; translate the camera while keeping the planet still
	// This will prevent OpenGL floating-point precision issues for objects far away from the global origin
	AstronomicalObject::SetTranslateWhenRendering(false);
	glLoadIdentity();
	Camera<double> cam(*camera);
	cam.SetPosition(cam.GetPosition() - astronomicalObject->GetPosition());
	cam.Use();
}

void SDLApplication::UseOrthogonalProjection() const {

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, windowWidth, 0, windowHeight);
	glScalef(1.0, -1.0, 1.0);
	glTranslatef(0.0, -(double) windowHeight, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void SDLApplication::RestoreProjection() const {

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void SDLApplication::Move(const double frameTime) {
}

#define min(a, b) (a < b? a : b)

double SDLApplication::GetDistanceToClosestAstronomicalObject() const {

	  return astronomicalObjects.size()
	    ? min(CAMERA_MAX_DISTANCE_TO_PLANET, astronomicalObjects[0]->GetClosestSurfaceDistance(camera->GetPosition()))
	    : CAMERA_MAX_DISTANCE_TO_PLANET;

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

struct AstronomicalObjectComparer {
	Vector3<double> cameraPosition;
	bool operator()(AstronomicalObject *a, AstronomicalObject *b) {
		return a->GetClosestSurfaceDistance(cameraPosition)
				< b->GetClosestSurfaceDistance(cameraPosition);
	}
} astronomicalObjectComparer;

bool GetLineSphereIntersection(const Vector3<double> &rayStart,
		const Vector3<double> &rayDir, const double sphereRadius, double &t0,
		double &t1) {
	const double b = 2.0 * rayStart.Dot(rayDir);
	const double c = rayStart.Dot(rayStart) - sphereRadius * sphereRadius;
	const double s = b * b - 4.0 * c;
	if (s < 0.0)
		return false;
	const double discriminant = sqrt(s);
	t0 = -0.5 * b - 0.5 * discriminant;
	t1 = -0.5 * b + 0.5 * discriminant;
	return true;
}

void SDLApplication::Update(const pssg::Time& dt) {

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

			SDL_GetMouseState(&mousePosInt.x, &mousePosInt.y);

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

		pssg::Time dt = clock.getTime();

		Update(dt);

		clock.restart();
	}

}

void SDLApplication::Render() const {
}
