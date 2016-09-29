/*
 * Color.h
 *
 *  Created on: 28 de out de 2015
 *      Author: cassiano
 */

#include <GL/gl.h>

#ifndef OBJECTVIEW_COLOR_H_
#define OBJECTVIEW_COLOR_H_

/**
 * Class that hold a rgb/rgba color.
 */
class Color {
public:
	/**
	 * Default constructor to white color.
	 */
	Color();

	/**
	 * Create a rgb color.
	 */
	Color(GLfloat r, GLfloat g, GLfloat b);

	/**
	 * Create a rgba color.
	 */
	Color(GLfloat r, GLfloat g, GLfloat b, GLfloat a);

	virtual ~Color();

	/**
	 * Get Red value.
	 */
	inline GLfloat r() const {
		return values[0];
	}

	/**
	 * Get Green value.
	 */
	inline GLfloat g() const {
		return values[1];
	}

	/**
	 * Get Blue value.
	 */
	inline GLfloat b() const {
		return values[2];
	}

	/**
	 * Get Alpha value.
	 */
	inline GLfloat a() const {
		return values[3];
	}

	/**
	 * Interpolate two colors.
	 * @param c1 First color
	 * @param c2 Second color
	 * @param factor Interpolation factor
	 */
	static Color interpolate(const Color& c1, const Color& c2, float factor);

	static Color WHITE;					/**< White color. */
	static Color BLACK;					/**< Black color. */
	static Color BLUE;					/**< Blue color. */
	static Color RED;					/**< Red color. */
	static Color GREEN;					/**< Green color. */

private:
	GLfloat values[4];
};

#endif /* OBJECTVIEW_COLOR_H_ */
