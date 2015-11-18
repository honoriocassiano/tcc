/*
 * Color.h
 *
 *  Created on: 28 de out de 2015
 *      Author: cassiano
 */

#include <GL/gl.h>

#ifndef OBJECTVIEW_COLOR_H_
#define OBJECTVIEW_COLOR_H_

namespace color {

class Color {
public:
	Color(GLfloat, GLfloat, GLfloat);
	Color(GLfloat, GLfloat, GLfloat, GLfloat);
	virtual ~Color();

	GLfloat getR() const { return r; }

	GLfloat getG() const { return g; }

	GLfloat getB() const { return b; }

	GLfloat getAlpha() const { return alpha; }

	static Color WHITE;
	static Color BLACK;
	static Color BLUE;
	static Color RED;
	static Color GREEN;

private:
	GLfloat r;
	GLfloat g;
	GLfloat b;
	GLfloat alpha;
};

} /* namespace words */

#endif /* OBJECTVIEW_COLOR_H_ */
