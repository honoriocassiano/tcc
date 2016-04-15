#ifndef SPHERE_H_
#define SPHERE_H_

#include <cmath>
#include <iostream>

//#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>

#include "Perlin.h"
#include "CelestialBody.h"

class Sphere : public CelestialBody {
public:
    Sphere(GLfloat radius, int horizontal_sections, int vertical_sections);
    virtual ~Sphere();

	virtual void show();
	virtual void update(float time);

private:
	static GLfloat to_rad;

	Vec3f center;
	Vec3f oldCenter;

	GLfloat radius;
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

//	Mesh* mesh;

	void makeVBO();
	void makePoints();
	void makeIndexes();
	void updateVertexes();

	void updatePoints();
};

#endif
