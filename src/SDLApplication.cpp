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

	Particle2D::DeleteAllParticles();
	QuadtreeTerrainNode::SetSplitDistanceScale(2.5);
	Planet::SetRenderOrbits(true);
	Planet::SetWireframe(false);
	camera->SetPosition(Vector3<double>(-120.0, 150.0, 420.0));
	camera->SetOrientation(
			Matrix3x3<double>::CreateRotationMatrixY(2.32)
					* Matrix3x3<double>::CreateRotationMatrixX(0.33));
	hoveredAstronomicalObjectIndex = -1;
	freezeLod = false;
	showControls = true;
	reverseTime = false;
	animationTimeUnit = ZERO_SECONDS;

	// Delete all astronomical objects
	for (unsigned int i = 0; i < astronomicalObjects.size(); i++)
		delete astronomicalObjects[i];
	astronomicalObjects.clear();

//	TODO Create the system

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

//void SDLApplication::Move(const pssg::Time& dt) {
//
//	const double frameTime = dt.getAsSeconds();
//
//	const double movementSpeed = 100.0
//			- 99.995 * exp(-0.01 * GetDistanceToClosestAstronomicalObject());
//
//	// Update the mouse position
//	static Vector2<int> mousePositionLast(0, 0);
//	Vector2<int> mousePosition;
////	glfwGetMousePos(&mousePosition.x, &mousePosition.y);
//	const auto keyboardState = SDL_GetKeyboardState(nullptr);
//	const auto mouseState = SDL_GetMouseState(&mousePosition.x,
//			&mousePosition.y);
//
//	const Vector2<int> mouseDelta = mousePosition - mousePositionLast;
//	mousePositionLast = mousePosition;
//
//	// Calculate camera velocity and rotation
//	const auto velocity = Vector3<double>(
//			keyboardState[SDL_SCANCODE_A] - keyboardState[SDL_SCANCODE_D], 0.0,
//			keyboardState[SDL_SCANCODE_W] - keyboardState[SDL_SCANCODE_S])
//			* movementSpeed;
//
//	const auto angularVelocity = Vector3<double>(
//			keyboardState[SDL_SCANCODE_UP] - keyboardState[SDL_SCANCODE_DOWN],
//			keyboardState[SDL_SCANCODE_LEFT]
//					- keyboardState[SDL_SCANCODE_RIGHT],
//			keyboardState[SDL_SCANCODE_E]
//					- keyboardState[SDL_SCANCODE_Q]) * CAMERA_ROTATION_SPEED;
//
//	Vector3<double> rotation(0.0);
//	Vector3<double> translation(0.0);
//
//	// If left-click dragging; rotate the camera
//	if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
//		rotation += Vector3<double>(mouseDelta.y, -mouseDelta.x,
//				0.0) * 0.005 * CAMERA_ROTATION_SPEED;
//	}
//
//	// If middle-click dragging; translate the camera
//	if (mouseState & SDL_BUTTON(SDL_BUTTON_MIDDLE)) {
//		translation += Vector3<double>(mouseDelta.x, mouseDelta.y, 0.0) * -0.01
//				* movementSpeed;
//	}
//
//	// If right-click dragging; translate the camera
//	if (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
//		translation.z += (mouseDelta.y + mouseDelta.x) * 0.05 * movementSpeed;
//	}
//
//	// Update the camera
//	camera->RotateEuler(angularVelocity * frameTime + rotation);
//	camera->Translate(velocity * frameTime + translation);
//
//	// If any objects exist (index 0 is the closest one after sorting)
//	if (astronomicalObjects.size() > 0) {
//		// If same position; move the camera a little (otherwise we won't know in which direction to move to get out)
//		if (camera->GetPosition() == astronomicalObjects[0]->GetPosition())
//			camera->Translate(
//					Vector3<double>(0.0, CAMERA_MIN_DISTANCE_TO_PLANET, 0.0));
//
//		// If camera below ground level; move the camera back to the planet's surface
//		const auto planetSurfaceDirection = (camera->GetPosition()
//				- astronomicalObjects[0]->GetPosition()).GetNormalized();
//
//		const auto planetSurfaceDistance =
//				astronomicalObjects[0]->GetClosestSurfaceDistance(
//						camera->GetPosition());
//
//		const auto cameraDistanceToPlanet = planetSurfaceDistance
//				- CAMERA_MIN_DISTANCE_TO_PLANET;
//
//		if (cameraDistanceToPlanet < 0.0)
//			camera->SetPosition(
//					camera->GetPosition()
//							- planetSurfaceDirection * cameraDistanceToPlanet);
//	}
//}

#define min(a, b) (a < b? a : b)

double SDLApplication::GetDistanceToClosestAstronomicalObject() const {

	return astronomicalObjects.size() ?
			min(CAMERA_MAX_DISTANCE_TO_PLANET,
					astronomicalObjects[0]->GetClosestSurfaceDistance(
							camera->GetPosition())) :
			CAMERA_MAX_DISTANCE_TO_PLANET;

}

void SDLApplication::ProcessRealtimeEvents(const pssg::Time& dt) {
//	Move(dt);
	const double frameTime = dt.getAsSeconds();

	const double movementSpeed = 100.0
			- 99.995 * exp(-0.01 * GetDistanceToClosestAstronomicalObject());

	// Update the mouse position
	static Vector2<int> mousePositionLast(0, 0);
	Vector2<int> mousePosition;
	//	glfwGetMousePos(&mousePosition.x, &mousePosition.y);
	const auto keyboardState = SDL_GetKeyboardState(nullptr);
	const auto mouseState = SDL_GetMouseState(&mousePosition.x,
			&mousePosition.y);

	const Vector2<int> mouseDelta = mousePosition - mousePositionLast;
	mousePositionLast = mousePosition;

	// Calculate camera velocity and rotation
	const auto velocity = Vector3<double>(
			keyboardState[SDL_SCANCODE_A] - keyboardState[SDL_SCANCODE_D], 0.0,
			keyboardState[SDL_SCANCODE_W] - keyboardState[SDL_SCANCODE_S])
			* movementSpeed;

	const auto angularVelocity = Vector3<double>(
			keyboardState[SDL_SCANCODE_UP] - keyboardState[SDL_SCANCODE_DOWN],
			keyboardState[SDL_SCANCODE_LEFT]
					- keyboardState[SDL_SCANCODE_RIGHT],
			keyboardState[SDL_SCANCODE_E]
					- keyboardState[SDL_SCANCODE_Q]) * CAMERA_ROTATION_SPEED;

	Vector3<double> rotation(0.0);
	Vector3<double> translation(0.0);

	// If left-click dragging; rotate the camera
	if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
		rotation += Vector3<double>(mouseDelta.y, -mouseDelta.x,
				0.0) * 0.005 * CAMERA_ROTATION_SPEED;
	}

	// If middle-click dragging; translate the camera
	if (mouseState & SDL_BUTTON(SDL_BUTTON_MIDDLE)) {
		translation += Vector3<double>(mouseDelta.x, mouseDelta.y, 0.0) * -0.01
				* movementSpeed;
	}

	// If right-click dragging; translate the camera
	if (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
		translation.z += (mouseDelta.y + mouseDelta.x) * 0.05 * movementSpeed;
	}

	// Update the camera
	camera->RotateEuler(angularVelocity * frameTime + rotation);
	camera->Translate(velocity * frameTime + translation);

	// If any objects exist (index 0 is the closest one after sorting)
	if (astronomicalObjects.size() > 0) {
		// If same position; move the camera a little (otherwise we won't know in which direction to move to get out)
		if (camera->GetPosition() == astronomicalObjects[0]->GetPosition())
			camera->Translate(
					Vector3<double>(0.0, CAMERA_MIN_DISTANCE_TO_PLANET, 0.0));

		// If camera below ground level; move the camera back to the planet's surface
		const auto planetSurfaceDirection = (camera->GetPosition()
				- astronomicalObjects[0]->GetPosition()).GetNormalized();

		const auto planetSurfaceDistance =
				astronomicalObjects[0]->GetClosestSurfaceDistance(
						camera->GetPosition());

		const auto cameraDistanceToPlanet = planetSurfaceDistance
				- CAMERA_MIN_DISTANCE_TO_PLANET;

		if (cameraDistanceToPlanet < 0.0)
			camera->SetPosition(
					camera->GetPosition()
							- planetSurfaceDirection * cameraDistanceToPlanet);
	}
}

void SDLApplication::ProcessEvent(const SDL_Event& e) {

	auto key = e.key.keysym.sym;
	const auto keyboardState = SDL_GetKeyboardState(nullptr);

	if (e.type == SDL_KEYUP) {
		switch (key) {
		case SDLK_F1:
			showControls = !showControls;
			break;
		case SDLK_F2:
			Planet::SetRenderOrbits(!Planet::GetRenderOrbits());
			break;
		case SDLK_F3:
			Planet::SetWireframe(!Planet::GetWireframe());
			break;
		case SDLK_F4:
			freezeLod = !freezeLod;
			break;
		case SDLK_F5:
			reverseTime = !reverseTime;
			break;
		case SDLK_0:
		case SDLK_1:
		case SDLK_2:
		case SDLK_3:
		case SDLK_4:
		case SDLK_5:
		case SDLK_6:
		case SDLK_7:
		case SDLK_8:
		case SDLK_9:
			animationTimeUnit = (TimeUnit) (SDL_GetKeyName(key)[0] - '0');
			break;
//		case GLFW_KEY_KP_ADD:
		case SDLK_KP_PLUS:
			QuadtreeTerrainNode::SetSplitDistanceScale(
					QuadtreeTerrainNode::GetSplitDistanceScale() + 0.1);
			break;
//		case GLFW_KEY_KP_SUBTRACT:
		case SDLK_KP_MINUS:
			QuadtreeTerrainNode::SetSplitDistanceScale(
					QuadtreeTerrainNode::GetSplitDistanceScale() - 0.1);
			break;
		case SDLK_DELETE:
			// Delete hovered object
			if (hoveredAstronomicalObjectIndex >= 0) {
				// Get object properties
				const Vector3<double> objectPosD =
						astronomicalObjects[hoveredAstronomicalObjectIndex]->GetPosition();
				const Vector3<float> objectPos((float) objectPosD.x,
						(float) objectPosD.y, (float) objectPosD.z);
				const float objectRadius =
						(float) astronomicalObjects[hoveredAstronomicalObjectIndex]->GetHoverBoundingRadius();

				// Delete object
				delete astronomicalObjects[hoveredAstronomicalObjectIndex];
				astronomicalObjects.erase(
						astronomicalObjects.begin()
								+ hoveredAstronomicalObjectIndex);
				hoveredAstronomicalObjectIndex = -1;

				// Create a fancy particle system in its place
				for (unsigned int i = 0; i < 1000; i++) {
					Vector3<float> randDir;
					Randomizer::GetDirection(randDir.x, randDir.y, randDir.z);
					new Particle2D(
							objectPos,                               // Position
							randDir
									* Randomizer::GetFloat(0.0f,
											7.0f * objectRadius), // Velocity
							0.0f,                                    // Friction
							Randomizer::GetFloat(0.0f, 360.0f),      // Rotation
							Randomizer::GetFloat(-720.0f, 720.0f), // Angular velocity
							0.0f,                           // Rotation friction
							Randomizer::GetFloat(0.05f, 0.1f) * objectRadius, // Scale
							0.0f,                                 // Scale decay
							Randomizer::GetFloat(0.0f, 0.5f));           // Life
				}
			}
			return;
		case SDLK_r:
			ResetScene();
			return;
		}

		// If Alt is being held; activate advanced controls
		if (keyboardState[SDL_SCANCODE_LALT]) {
			// If an object is being hovered
			if (hoveredAstronomicalObjectIndex >= 0) {
				// If the hovered object is a planet
				if (Planet *planet =
						dynamic_cast<Planet*>(astronomicalObjects[hoveredAstronomicalObjectIndex])) {
					// Determine if any property should be changed
					const bool changeHeightScale = key == SDLK_t;
					const bool changeOctaves = key == SDLK_o;
					const bool changeGain = key == SDLK_g;
					const bool changeLacunarity = key == SDLK_l;
					const bool changeOffset = key == SDLK_f;
					const bool changeH = key == SDLK_h;
					if (changeHeightScale || changeOctaves || changeGain
							|| changeLacunarity || changeOffset || changeH) {
						// Shift reverses the effect
						const bool reverse = keyboardState[SDL_SCANCODE_LSHIFT];

						// Get current properties
						double heightScale, gain, lacunarity, offset, h;
						int octaves;
						planet->GetTerrain()->GetRidgedMultifractalParameters(
								heightScale, octaves, gain, lacunarity, offset,
								h);

						// Change properties
						if (changeHeightScale)
							heightScale += reverse ? -0.001 : 0.001;
						if (changeOctaves)
							octaves += reverse ? -1 : 1;
						if (changeGain)
							gain += reverse ? -0.1 : 0.1;
						if (changeLacunarity)
							lacunarity += reverse ? -0.01 : 0.01;
						if (changeOffset)
							offset += reverse ? -0.01 : 0.01;
						if (changeH)
							h += reverse ? -0.01 : 0.01;

						// Update properties
						planet->GetTerrain()->SetRidgedMultifractalParameters(
								heightScale, octaves, gain, lacunarity, offset,
								h);
						planet->GetTerrain()->Rebuild();
					}
				}
			}
		}
	}

//	if (action == GLFW_PRESS) {
//		switch (key) {
//		case GLFW_KEY_F1:
//			showControls = !showControls;
//			break;
//		case GLFW_KEY_F2:
//			Planet::SetRenderOrbits(!Planet::GetRenderOrbits());
//			break;
//		case GLFW_KEY_F3:
//			Planet::SetWireframe(!Planet::GetWireframe());
//			break;
//		case GLFW_KEY_F4:
//			freezeLod = !freezeLod;
//			break;
//		case GLFW_KEY_F5:
//			reverseTime = !reverseTime;
//			break;
//		case '0':
//		case '1':
//		case '2':
//		case '3':
//		case '4':
//		case '5':
//		case '6':
//		case '7':
//		case '8':
//		case '9':
//			animationTimeUnit = (TimeUnit) (key - '0');
//			break;
//		case GLFW_KEY_KP_ADD:
//			QuadtreeTerrainNode::SetSplitDistanceScale(
//					QuadtreeTerrainNode::GetSplitDistanceScale() + 0.1);
//			break;
//		case GLFW_KEY_KP_SUBTRACT:
//			QuadtreeTerrainNode::SetSplitDistanceScale(
//					QuadtreeTerrainNode::GetSplitDistanceScale() - 0.1);
//			break;
//		case GLFW_KEY_DEL:
//			// Delete hovered object
//			if (hoveredAstronomicalObjectIndex >= 0) {
//				// Get object properties
//				const Vector3<double> objectPosD =
//						astronomicalObjects[hoveredAstronomicalObjectIndex]->GetPosition();
//				const Vector3<float> objectPos((float) objectPosD.x,
//						(float) objectPosD.y, (float) objectPosD.z);
//				const float objectRadius =
//						(float) astronomicalObjects[hoveredAstronomicalObjectIndex]->GetHoverBoundingRadius();
//
//				// Delete object
//				delete astronomicalObjects[hoveredAstronomicalObjectIndex];
//				astronomicalObjects.erase(
//						astronomicalObjects.begin()
//								+ hoveredAstronomicalObjectIndex);
//				hoveredAstronomicalObjectIndex = -1;
//
//				// Create a fancy particle system in its place
//				for (unsigned int i = 0; i < 1000; i++) {
//					Vector3<float> randDir;
//					Randomizer::GetDirection(randDir.x, randDir.y, randDir.z);
//					new Particle2D(
//							objectPos,                               // Position
//							randDir
//									* Randomizer::GetFloat(0.0f,
//											7.0f * objectRadius), // Velocity
//							0.0f,                                    // Friction
//							Randomizer::GetFloat(0.0f, 360.0f),      // Rotation
//							Randomizer::GetFloat(-720.0f, 720.0f), // Angular velocity
//							0.0f,                           // Rotation friction
//							Randomizer::GetFloat(0.05f, 0.1f) * objectRadius, // Scale
//							0.0f,                                 // Scale decay
//							Randomizer::GetFloat(0.0f, 0.5f));           // Life
//				}
//			}
//			return;
//		case 'R':
//			ResetScene();
//			return;
//		}
//
//		// If Alt is being held; activate advanced controls
//		if (glfwGetKey(GLFW_KEY_LALT)) {
//			// If an object is being hovered
//			if (hoveredAstronomicalObjectIndex >= 0) {
//				// If the hovered object is a planet
//				if (Planet *planet =
//						dynamic_cast<Planet*>(astronomicalObjects[hoveredAstronomicalObjectIndex])) {
//					// Determine if any property should be changed
//					const bool changeHeightScale = key == 'T';
//					const bool changeOctaves = key == 'O';
//					const bool changeGain = key == 'G';
//					const bool changeLacunarity = key == 'L';
//					const bool changeOffset = key == 'F';
//					const bool changeH = key == 'H';
//					if (changeHeightScale || changeOctaves || changeGain
//							|| changeLacunarity || changeOffset || changeH) {
//						// Shift reverses the effect
//						const bool reverse = glfwGetKey(GLFW_KEY_LSHIFT);
//
//						// Get current properties
//						double heightScale, gain, lacunarity, offset, h;
//						int octaves;
//						planet->GetTerrain()->GetRidgedMultifractalParameters(
//								heightScale, octaves, gain, lacunarity, offset,
//								h);
//
//						// Change properties
//						if (changeHeightScale)
//							heightScale += reverse ? -0.001 : 0.001;
//						if (changeOctaves)
//							octaves += reverse ? -1 : 1;
//						if (changeGain)
//							gain += reverse ? -0.1 : 0.1;
//						if (changeLacunarity)
//							lacunarity += reverse ? -0.01 : 0.01;
//						if (changeOffset)
//							offset += reverse ? -0.01 : 0.01;
//						if (changeH)
//							h += reverse ? -0.01 : 0.01;
//
//						// Update properties
//						planet->GetTerrain()->SetRidgedMultifractalParameters(
//								heightScale, octaves, gain, lacunarity, offset,
//								h);
//						planet->GetTerrain()->Rebuild();
//					}
//				}
//			}
//		}
//	}

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
	delete camera;
	delete textTool;
	delete skybox;
	delete starfield;

	// Delete all astronomical objects
	for (unsigned int i = 0; i < astronomicalObjects.size(); i++)
		delete astronomicalObjects[i];
	astronomicalObjects.clear();

	// Deinitialize
	Particle2D::DeleteAllParticles();
	ShaderManager::Deinitialize();

	// Delete permutation texture
	const GLuint permTex = PerlinNoise<double>::GetPermutationTexture();
	glDeleteTextures(1, &permTex);
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
//	Move(dt);

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

		pssg::Time dt = clock.getTime();

		ProcessRealtimeEvents(dt);

		SDL_Event e;

		while (SDL_PollEvent(&e)) {
			ProcessEvent(e);
		}

		Update(dt);

		clock.restart();
	}

}

void SDLApplication::Render() const {
}
