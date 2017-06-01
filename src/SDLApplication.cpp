/*
 * SDLApplication.cpp
 *
 *  Created on: 17 de mai de 2017
 *      Author: cassiano
 */

#include "SDLApplication.h"

SDLApplication::SDLApplication(int _width, int _height) :
		windowWidth(_width), windowHeight(_height), isRunning(false) {

	if (InitSDL()) {

		PerlinNoise<double>::GetPermutationTexture();
		ShaderManager::Initialize();

		// Create objects
		fpsCounter = new FpsCounter<double>;
		camera = new Camera<double>();
		textTool = new TextTool(2.0);
		skybox = new Skybox;
		starfield = new Starfield(4096);

		// Initialize the scene
		ResetScene();
	}
}

void SDLApplication::RenderSystem() {
	Star *sun = new Star(50.0);
	sun->SetName("Star");
	astronomicalObjects.push_back(sun);

	// Create a Mercury-like planet
	Planet *freddie = new Planet(2.4397);
	freddie->SetMass(3.3022e5);
	freddie->SetName("Freddie");
	freddie->SetParent(sun);
	freddie->GetTerrain()->SetRidgedMultifractalParameters(0.035, 20, 2.1, 2.01,
			1.0, 0.9);
	freddie->SetAnimationOrbit(Matrix3x3<double>::CreateRotationMatrixZ(0.09),
			280.0, 0.05, 8.27e-7);
	freddie->SetAnimationRotation(
			Matrix3x3<double>::CreateRotationMatrixX(0.037), 0.5, 1.24e-6);
	vector<Vector4<double> > freddieColors;
	freddieColors.push_back(Vector4<double>(0.80, 0.82, 0.80, 0.05));
	freddieColors.push_back(Vector4<double>(0.75, 0.70, 0.75, 0.55));
	freddieColors.push_back(Vector4<double>(0.85, 0.90, 0.95, 0.95));
	freddie->BuildColormap(freddieColors);
	astronomicalObjects.push_back(freddie);

	// Create an Earth-like planet
	Planet *procedurus = new Planet(6.3710);
	procedurus->SetMass(5.9736e6);
	procedurus->SetName("Procedurus");
	procedurus->SetParent(sun);
	procedurus->GetTerrain()->SetRidgedMultifractalParameters(0.03, 20, 1.6,
			2.5, 1.0, 0.87);
	procedurus->SetAnimationOrbit(Matrix3x3<double>::CreateIdentityMatrix(),
			420.0, 0.0, 1.99e-7);
	procedurus->SetAnimationRotation(
			Matrix3x3<double>::CreateRotationMatrixZ(-0.41), 0.0, 7.29e-5);
	procedurus->SetAtmosphere(7.2, Vector3<float>(0.5, 0.6, 1.0));
	procedurus->SetClouds(6.56213, 0.6, 0.01);
	vector<Vector4<double> > procedurusColors;
	procedurusColors.push_back(Vector4<double>(0.00, 0.00, 0.20, 0.05));
	procedurusColors.push_back(Vector4<double>(0.00, 0.00, 0.40, 0.35));
	procedurusColors.push_back(Vector4<double>(0.00, 0.00, 1.00, 0.45));
	procedurusColors.push_back(Vector4<double>(1.00, 1.00, 0.80, 0.52));
	procedurusColors.push_back(Vector4<double>(0.10, 0.50, 0.10, 0.55));
	procedurusColors.push_back(Vector4<double>(0.10, 0.40, 0.10, 0.65));
	procedurusColors.push_back(Vector4<double>(0.40, 0.40, 0.20, 0.70));
	procedurusColors.push_back(Vector4<double>(0.60, 0.70, 0.60, 0.85));
	procedurusColors.push_back(Vector4<double>(0.90, 0.95, 0.95, 0.92));
	procedurus->BuildColormap(procedurusColors);
	astronomicalObjects.push_back(procedurus);

	// Create a Luna-like planet
	Planet *runar = new Planet(1.737);
	runar->SetMass(7.3477e4);
	runar->SetName("Runar");
	runar->SetParent(procedurus);
	runar->GetTerrain()->SetRidgedMultifractalParameters(0.02, 20, 1.8, 2.5,
			1.0, 0.85);
	runar->SetAnimationOrbit(Matrix3x3<double>::CreateRotationMatrixZ(0.09),
			50.0, 0.2, 2.66e-6);
	runar->SetAnimationRotation(Matrix3x3<double>::CreateRotationMatrixZ(0.029),
			0.0, 2.66e-6);
	vector<Vector4<double> > runarColors;
	runarColors.push_back(Vector4<double>(0.25, 0.25, 0.25, 0.05));
	runarColors.push_back(Vector4<double>(0.95, 0.95, 0.95, 0.95));
	runar->BuildColormap(runarColors);
	astronomicalObjects.push_back(runar);

	// Create a Mars-like planet
	Planet *ares = new Planet(3.3962);
	ares->SetMass(6.4185e5);
	ares->SetName("Ares");
	ares->SetParent(sun);
	ares->GetTerrain()->SetRidgedMultifractalParameters(0.028, 20, 2.4, 1.95,
			1.0, 0.85);
	ares->SetAnimationOrbit(Matrix3x3<double>::CreateRotationMatrixZ(0.035),
			520.0, -3.0, 1.06e-7);
	ares->SetAnimationRotation(Matrix3x3<double>::CreateRotationMatrixX(0.44),
			0.2, 7.09e-5);
	vector<Vector4<double> > aresColors;
	aresColors.push_back(Vector4<double>(0.50, 0.25, 0.25, 0.05));
	aresColors.push_back(Vector4<double>(0.95, 0.50, 0.40, 0.95));
	ares->BuildColormap(aresColors);
	astronomicalObjects.push_back(ares);

	// Create a ringed planet
	Planet *ringo = new Planet(60.268);
	ringo->SetMass(5.6846e8);
	ringo->SetName("Ringo");
	ringo->SetParent(sun);
	ringo->GetTerrain()->SetRidgedMultifractalParameters(0.005, 16, 1.5, 2.1,
			1.0, 1.0);
	ringo->SetAnimationOrbit(Matrix3x3<double>::CreateRotationMatrixZ(-0.038),
			800.0, -0.2, 6.76e-9);
	ringo->SetAnimationRotation(
			Matrix3x3<double>::CreateRotationMatrixY(0.5)
					* Matrix3x3<double>::CreateRotationMatrixZ(-0.467), 0.2,
			1.92e-7);
	ringo->SetRing(68.0, 128.0, Vector4<double>(0.6, 0.8, 1.0, 1.0));
	vector<Vector4<double> > ringoColors;
	ringoColors.push_back(Vector4<double>(0.60, 0.75, 0.95, 0.05));
	ringoColors.push_back(Vector4<double>(0.70, 0.80, 0.95, 0.95));
	ringo->BuildColormap(ringoColors);
	astronomicalObjects.push_back(ringo);

	// Create a distant planet
	Planet *xb360 = new Planet(25.0);
	xb360->SetMass(1.0e8);
	xb360->SetName("XB-360");
	xb360->SetParent(sun);
	xb360->GetTerrain()->SetRidgedMultifractalParameters(0.03, 20, 2.2, 2.01,
			1.0, 0.90);
	xb360->SetAnimationOrbit(Matrix3x3<double>::CreateRotationMatrixX(-0.04),
			1100.0, -0.2, 1.65e-9);
	xb360->SetAnimationRotation(
			Matrix3x3<double>::CreateRotationMatrixY(-0.5)
					* Matrix3x3<double>::CreateRotationMatrixZ(-0.8), 0.2,
			1.0e-5);
	vector<Vector4<double> > xb360Colors;
	xb360Colors.push_back(Vector4<double>(0.10, 0.10, 0.15, 0.00));
	xb360Colors.push_back(Vector4<double>(0.10, 0.15, 0.25, 0.70));
	xb360Colors.push_back(Vector4<double>(0.50, 0.45, 0.85, 1.00));
	xb360->BuildColormap(xb360Colors);
	astronomicalObjects.push_back(xb360);
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
	for (unsigned int i = 0; i < astronomicalObjects.size(); i++) {
		delete astronomicalObjects[i];
	}
	astronomicalObjects.clear();

	RenderSystem();
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
		if (camera->GetPosition() == astronomicalObjects[0]->GetPosition()) {
			camera->Translate(
					Vector3<double>(0.0, CAMERA_MIN_DISTANCE_TO_PLANET, 0.0));
		}

		// If camera below ground level; move the camera back to the planet's surface
		const auto planetSurfaceDirection = (camera->GetPosition()
				- astronomicalObjects[0]->GetPosition()).GetNormalized();

		const auto planetSurfaceDistance =
				astronomicalObjects[0]->GetClosestSurfaceDistance(
						camera->GetPosition());

		const auto cameraDistanceToPlanet = planetSurfaceDistance
				- CAMERA_MIN_DISTANCE_TO_PLANET;

		if (cameraDistanceToPlanet < 0.0) {
			camera->SetPosition(
					camera->GetPosition()
							- planetSurfaceDirection * cameraDistanceToPlanet);
		}
	}
}

void SDLApplication::ProcessEvent(const SDL_Event& e) {

	const auto keyboardState = SDL_GetKeyboardState(nullptr);

	if (e.type == SDL_WINDOWEVENT) {

		if (e.window.event == SDL_WINDOWEVENT_CLOSE) {
			isRunning = false;
		}

	} else if (e.type == SDL_KEYUP) {

		auto key = e.key.keysym.sym;

		switch (key) {

		case SDLK_ESCAPE:
			isRunning = false;
			break;
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
}

bool SDLApplication::InitSDL() {

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Error initializing SDL: %s\n", SDL_GetError());
		return false;
	}

	window = SDL_CreateWindow("TCC", SDL_WINDOWPOS_UNDEFINED,
	SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_OPENGL);

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

void SDLApplication::Run() {

	isRunning = true;

	clock.start();

	while (isRunning) {

		pssg::Time dt = clock.getTime();
		clock.restart();

		ProcessRealtimeEvents(dt);

		SDL_Event e;

		while (SDL_PollEvent(&e)) {
			ProcessEvent(e);
		}

		Update(dt);

		Render();

//		clock.restart();
	}
}

void SDLApplication::Render() const {
	glViewport(0, 0, windowWidth, windowHeight);

	// Set the clear color and clear the color and depth buffers before rendering
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glColor4d(1.0, 1.0, 1.0, 1.0);

	// Enable/disable server-side GL capabilities
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	// Render skybox and starfield
	UsePerspectiveProjection(0.1, 2.0);
	skybox->Render();
	starfield->Render();
	RestoreProjection();

	// Set projection
	glClear(GL_DEPTH_BUFFER_BIT);
	UsePerspectiveProjection(1.0, 10000.0);

	// Disable shader program
	glUseProgram(0);

	// Set line rendering properties
	glLineWidth(2.0);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	// Prepare to render semitransparent particles and antialiased lines
	glPushAttrib(GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Render orbits
	if (Planet::GetRenderOrbits()) {
		for (unsigned int i = 0; i < astronomicalObjects.size(); i++) {
			if (Planet *planet = dynamic_cast<Planet*>(astronomicalObjects[i])) {
				planet->RenderOrbit();
			}
		}
	}

	// Render particle system
	Particle2D::RenderAllParticles();

	glPopAttrib();
	RestoreProjection();

	// Set line rendering properties
	glLineWidth(1.0);
	glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);

	// Render astronomical objects back-to-front
	for (int i = astronomicalObjects.size() - 1; i >= 0; i--) {
		// Render planet
		glClear(GL_DEPTH_BUFFER_BIT);
		UsePerspectiveProjection(astronomicalObjects[i]);
		astronomicalObjects[i]->Render();
		RestoreProjection();
	}

	// Prepare to render text
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	UseOrthogonalProjection();
	glColor4d(1.0, 1.0, 1.0, 1.0);

//	if (showControls) {
//		char strInfo[1024];
//		sprintf(strInfo, "BASIC CONTROLS                        FPS: %d\n"
//				"===============================================\n"
//				"* W/A/S/D or MMB/RMB + drag to move\n"
//				"* E/Q, arrow keys or LMB + drag to look around\n"
//				"===============================================\n"
//				" F1: Show Controls       [%d]\n"
//				" F2: Render Orbits       [%d]\n"
//				" F3: Toggle Wireframe    [%d]\n"
//				" F4: Freeze LOD Updates  [%d]\n"
//				" F5: Reverse Time        [%d]\n"
//				"0-9: Set Animation Speed [%s per second]\n"
//				"+/-: Set Split Distance  [%.1f]\n"
//				"Del: Destroy Object\n"
//				"  R: Reset Scene\n", (unsigned int) fpsCounter->GetFps(),
//				showControls, Planet::GetRenderOrbits(), Planet::GetWireframe(),
//				freezeLod, reverseTime,
//				timeUnitNames[animationTimeUnit].c_str(),
//				QuadtreeTerrainNode::GetSplitDistanceScale());
//		textTool->RenderText(strInfo, 1, 1);
//
//		textTool->RenderText("ADVANCED CONTROLS\n"
//				"========================\n"
//				"* Hover a planet\n"
//				"* Hold Alt to activate\n"
//				"* Hold Shift to reverse\n"
//				"========================\n"
//				"T: Increase Height Scale\n"
//				"O: Increase # of Octaves\n"
//				"G: Increase Gain\n"
//				"L: Increase Lacunarity\n"
//				"F: Increase Offset\n"
//				"H: Increase H Value\n", windowWidth - 291, 1);
//	}

//	// If an object is hovered
//	if (hoveredAstronomicalObjectIndex >= 0 && Planet::GetRenderOrbits()) {
//		Vector2<int> mousePosition;
//		SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
//		textTool->RenderText(
//				astronomicalObjects[hoveredAstronomicalObjectIndex]->GetString().c_str(),
//				mousePosition.x + 14, mousePosition.y);
//	}

	RestoreProjection();
	glPopAttrib();

	// Swap the front buffer with the back buffer
	SDL_GL_SwapWindow(window);
}
