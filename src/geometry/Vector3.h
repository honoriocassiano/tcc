/*
 * Vector.h
 *
 *  Created on: 28 de ouGLfloat de 2015
 *      Author: cassiano
 */

#include <GL/gl.h>
#include <cmath>

#ifndef GEOMETRY_VECTOR3_H_
#define GEOMETRY_VECTOR3_H_

namespace geometry {

class Vector3 {
public:
	Vector3(GLfloat,GLfloat,GLfloat);
	virtual ~Vector3();

	Vector3 operator+(Vector3);
	Vector3 operator-(Vector3);
	GLfloat operator*(Vector3);

	virtual GLfloat magnitude();
	virtual void normalize();

	GLfloat getX();
	GLfloat getY();
	GLfloat getZ();

private:
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

} /* namespace words */

#endif /* GEOMETRY_VECTOR3_H_ */
