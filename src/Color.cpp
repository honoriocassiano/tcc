/*
 * Color.cpp
 *
 *  Created on: 28 de out de 2015
 *      Author: cassiano
 */

#include "Color.h"

Color Color::WHITE = Color(1.0f, 1.0f, 1.0f);
Color Color::BLACK = Color(0.0f, 0.0f, 0.0f);
Color Color::BLUE = Color(0.0f, 0.0f, 1.0f);
Color Color::RED = Color(1.0f, 0.0f, 0.0f);
Color Color::GREEN = Color(0.0f, 1.0f, 0.0f);

Color::Color(GLfloat r, GLfloat g, GLfloat b) {
	this->mR = r;
	this->mG = g;
	this->mB = b;
	this->mAlpha = 1.0f;
}

Color::Color(GLfloat r, GLfloat g, GLfloat b, GLfloat alpha) {
	this->mR = r;
	this->mG = g;
	this->mB = b;
	this->mAlpha = alpha;
}

Color::Color() {
	this->mR = 1.0f;
	this->mG = 1.0f;
	this->mB = 1.0f;
	this->mAlpha = 1.0f;
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
