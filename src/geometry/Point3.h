/*
 * Point3.h
 *
 *  Created on: 29 de out de 2015
 *      Author: cassiano
 */

#include <GL/gl.h>

#ifndef GEOMETRY_POINT3_H_
#define GEOMETRY_POINT3_H_

namespace geometry {

class Point3 {
public:
	explicit Point3(GLfloat, GLfloat, GLfloat);
	virtual ~Point3();

	GLfloat getX() const {
		return x;
	}

	GLfloat getY() const {
		return y;
	}

	GLfloat getZ() const {
		return z;
	}

private:
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

} /* namespace words */

#endif /* GEOMETRY_POINT3_H_ */
