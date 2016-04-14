/*
 * Vector.h
 *
 *  Created on: 29 de out de 2015
 *      Author: cassiano
 */

#include <GL/gl.h>

#ifndef GEOMETRY_VECTOR_H_
#define GEOMETRY_VECTOR_H_

namespace geometry {

class Vector {
public:
	Vector();
	virtual ~Vector();

	virtual GLfloat magnitude() = 0;
	virtual void normalize() = 0;
};

} /* namespace words */

#endif /* GEOMETRY_VECTOR_H_ */
