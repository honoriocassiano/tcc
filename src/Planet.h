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
	Planet(GLfloat radius, int horizontal_sections, int vertical_sections);
	virtual ~Planet();

	virtual void show();

	float getRadius() const {
		return mRadius;
	}
//	virtual void update(float time);

	void update(const Time& dt) override;
	void draw() override;

private:
	static GLfloat to_rad;

	Vec3f center;
	Vec3f oldCenter;

	GLfloat mRadius;
	int h_sections;
	int v_sections;

	int vertex_size;
	int index_size;

	GLfloat step_circle_angle;
	GLfloat step_height_angle;

	GLuint vertex_id;
	GLuint vertex_index_id;

	GLfloat * vertex;
	GLuint * vertex_index;

	void subdivide();

	void makePoints();
	void makeTriangles();
	void updatePoints();

	const static GLfloat vdata[12][3];
	const static GLuint tindices[20][3];

	float mTurbulenceDistortion;

	bool mPendingUpdate;

	static int octaves;
	static int A;
	static int B;
};

#endif
