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
		return mRadius;
	}

	void update(const Time& dt) override;
	void draw() override;

private:
	static GLfloat to_rad;

	GLfloat mRadius;

	Vec3f mOrbitGravity;

	void subdivide();

	void makePoints();
	void makeTriangles();
	//void updatePoints();

	void calculateOrbitGravity();

	const static GLfloat vdata[12][3];
	const static GLuint tindices[20][3];

	float mTurbulenceDistortion;

	bool mPendingUpdate;

	static int octaves;
	static int A;
	static int B;
};

#endif
