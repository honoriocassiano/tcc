#ifndef PLANET_H_
#define PLANET_H_

#include <cmath>
#include <iostream>

#include <GL/gl.h>
#include <GL/glut.h>

#include "Perlin.h"
#include "CelestialBody.h"

class Planet: public CelestialBody {
public:
	Planet(const Vec3f& center, float radius);
	virtual ~Planet();

	void update(const Time& dt) override;

private:
	static float to_rad;

	Vec3f orbitGravity;

	void calculateOrbitGravity();

	float turbulenceDistortion;

	bool pendingUpdate;
};

#endif
