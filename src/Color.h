/*
 * Color.h
 *
 *  Created on: 28 de out de 2015
 *      Author: cassiano
 */

#include <GL/gl.h>

#ifndef OBJECTVIEW_COLOR_H_
#define OBJECTVIEW_COLOR_H_

class Color {
public:
	// Default color is white
	Color();
	Color(GLfloat, GLfloat, GLfloat);
	Color(GLfloat, GLfloat, GLfloat, GLfloat);
	virtual ~Color();

	GLfloat r() const { return mR; }

	GLfloat g() const { return mG; }

	GLfloat b() const { return mB; }

	GLfloat a() const { return mAlpha; }

	static Color interpolate(const Color& c1, const Color& c2, float factor);

	static Color WHITE;
	static Color BLACK;
	static Color BLUE;
	static Color RED;
	static Color GREEN;

private:
	GLfloat mR;
	GLfloat mG;
	GLfloat mB;
	GLfloat mAlpha;
};

#endif /* OBJECTVIEW_COLOR_H_ */
