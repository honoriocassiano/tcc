/*
 * SDLApplication.h
 *
 *  Created on: 17 de mai de 2017
 *      Author: cassiano
 */

#ifndef SRC_SDLAPPLICATION_H_
#define SRC_SDLAPPLICATION_H_

#include "Quadtree/Shaders/ShaderManager.h"
#include "Quadtree/Geometry/Skybox.h"
#include "Quadtree/Geometry/Planet.h"
#include "Quadtree/Geometry/Star.h"
#include "Quadtree/Geometry/Starfield.h"
#include "Quadtree/Geometry/Particle2D.h"
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

enum TimeUnit { ZERO_SECONDS, ONE_SECOND, ONE_MINUTE, ONE_HOUR, ONE_DAY, ONE_WEEK, ONE_MONTH, ONE_YEAR, ONE_DECADE, ONE_CENTURY };
const double timeUnitSeconds[10] = { 0.0, 1.0, 60.0, 3600.0, 86400.0, 604800.0, 2629743.83, 31556926.0, 315569260.0, 3155692600.0 };
const string timeUnitNames[10] = { "0 seconds", "1 second", "1 minute", "1 hour", "1 day", "1 week", "1 month", "1 year", "1 decade", "1 century" };

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
	bool isRunning;
	pssg::Clock clock;


	SDL_Window* window;
	SDL_GLContext glContext;

	void RenderSystem();
	void ResetScene();
	void UsePerspectiveProjection(const double zNear, const double zFar) const;
	void UsePerspectiveProjection(
			const AstronomicalObject *astronomicalObject) const;
	void UseOrthogonalProjection() const;
	void RestoreProjection() const;
	double GetDistanceToClosestAstronomicalObject() const;
	void ProcessRealtimeEvents(const pssg::Time& dt);
	void ProcessEvent(const SDL_Event& event);

	bool InitSDL();

public:
	SDLApplication(int width, int height);
	virtual ~SDLApplication();

	void Run();

	void Update(const pssg::Time& dt);
	void Render() const;
};

#endif /* SRC_SDLAPPLICATION_H_ */
