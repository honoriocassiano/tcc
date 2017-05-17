/*
 * SDLApplication.h
 *
 *  Created on: 17 de mai de 2017
 *      Author: cassiano
 */

#ifndef SRC_SDLAPPLICATION_H_
#define SRC_SDLAPPLICATION_H_

#include "Shaders/ShaderManager.h"
#include "Geometry/Skybox.h"
#include "Geometry/Planet.h"
#include "Geometry/Star.h"
#include "Geometry/Starfield.h"
#include "Geometry/Particle2D.h"
#include "Quadtree/Application/TextTool.h"
#include "Quadtree/Math/VectorMath.h"
#include "Quadtree/Math/PerlinNoise.h"
#include "Quadtree/Math/Randomizer.h"
#include "Quadtree/Application/Camera.h"
#include "Quadtree/Application/FpsCounter.h"
#include <stdio.h>

#include "Clock.h"

#include <SDL2/SDL.h>

#define CAMERA_MAX_DISTANCE_TO_PLANET 1000.0
#define CAMERA_MIN_DISTANCE_TO_PLANET 0.0004
#define CAMERA_ROTATION_SPEED         1.0

class SDLApplication {

private:
	int windowWidth, windowHeight;
	Camera<double> *camera;
	FpsCounter<double> *fpsCounter;
	TextTool *textTool;
	Skybox *skybox;
	std::vector<AstronomicalObject*> astronomicalObjects;
	Starfield *starfield;
	int hoveredAstronomicalObjectIndex;
	bool freezeLod;
	bool showControls;
	bool reverseTime;
	TimeUnit animationTimeUnit;
	int width;
	int height;
	bool isRunning;
	Clock clock;


	SDL_Window* window;
	SDL_GLContext glContext;

	void ResetScene();
	void UsePerspectiveProjection(const double zNear, const double zFar) const;
	void UsePerspectiveProjection(
			const AstronomicalObject *astronomicalObject) const;
	void UseOrthogonalProjection() const;
	void RestoreProjection() const;
	void Move(const double frameTime);
	double GetDistanceToClosestAstronomicalObject() const;
	void ProcessRealtimeEvents();
	void ProcessEvent(SDL_Event& event);

	bool InitSDL();
	bool InitGL();

public:
	SDLApplication(int width, int height);
	virtual ~SDLApplication();

	void Run();

	void Update(const Time& dt);
	void Render() const;
};

#endif /* SRC_SDLAPPLICATION_H_ */
