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
	Planet(GLfloat radius);
	virtual ~Planet();

	float getRadius() const {
		return radius;
	}

	void update(const Time& dt) override;
	void draw() override;

private:
	static GLfloat to_rad;

	GLfloat radius;

	Vec3f orbitGravity;

	void calculateOrbitGravity();

	float turbulenceDistortion;

	bool pendingUpdate;
};

#endif
