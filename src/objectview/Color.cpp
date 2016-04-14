/*
 * Color.cpp
 *
 *  Created on: 28 de out de 2015
 *      Author: cassiano
 */

#include "Color.h"

namespace color {

Color Color::WHITE = Color(1.0f, 1.0f, 1.0f);
Color Color::BLACK = Color(0.0f, 0.0f, 0.0f);
Color Color::BLUE = Color(0.0f, 0.0f, 1.0f);
Color Color::RED = Color(1.0f, 0.0f, 0.0f);
Color Color::GREEN = Color(0.0f, 1.0f, 0.0f);

Color::Color(GLfloat r, GLfloat g, GLfloat b) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->alpha = 1.0f;
}

Color::Color(GLfloat r, GLfloat g, GLfloat b, GLfloat alpha) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->alpha = alpha;
}

Color::~Color() {
	// TODO Auto-generated destructor stub
}

} /* namespace color */

