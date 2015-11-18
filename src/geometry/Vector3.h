/*
 * Vector.h
 *
 *  Created on: 28 de ouGLfloat de 2015
 *      Author: cassiano
 */

#include "Vector.h"
#include <math.h>

#ifndef GEOMETRY_VECTOR3_H_
#define GEOMETRY_VECTOR3_H_

namespace geometry {

class Vector3 : public Vector {
public:
	Vector3(GLfloat x,GLfloat y,GLfloat z);
	virtual ~Vector3();

	Vector3 operator+(Vector3 other);
	Vector3 operator-(Vector3 other);
	GLfloat operator*(Vector3 other);

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

} /* namespace geometry */

#endif /* GEOMETRY_VECTOR3_H_ */
