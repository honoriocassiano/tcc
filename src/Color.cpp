/*
 * Color.cpp
 *
 *  Created on: 28 de out de 2015
 *      Author: cassiano
 */

#include "Color.h"

Color Color::WHITE = Color(1.0f, 1.0f, 1.0f);			/**< @hideinitializer */
Color Color::BLACK = Color(0.0f, 0.0f, 0.0f);			/**< @hideinitializer */
Color Color::BLUE = Color(0.0f, 0.0f, 1.0f);			/**< @hideinitializer */
Color Color::RED = Color(1.0f, 0.0f, 0.0f);				/**< @hideinitializer */
Color Color::GREEN = Color(0.0f, 1.0f, 0.0f);			/**< @hideinitializer */

Color::Color(GLfloat _r, GLfloat _g, GLfloat _b) :
		values { _r, _g, _b, 1.0f } {

}

Color::Color(GLfloat _r, GLfloat _g, GLfloat _b, GLfloat _alpha) :
		values { _r, _g, _b, _alpha } {
}

Color::Color() :
		values { 1.0f, 1.0f, 1.0f, 1.0f } {
}

Color::~Color() {
	// TODO Auto-generated destructor stub
}

Color Color::interpolate(const Color& c1, const Color& c2, float factor) {
	float r = c1.r() * factor + c2.r() * (1 - factor);
	float g = c1.g() * factor + c2.g() * (1 - factor);
	float b = c1.b() * factor + c2.b() * (1 - factor);

	return Color(r, g, b);
}
