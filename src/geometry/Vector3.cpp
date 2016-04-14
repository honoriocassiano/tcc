/*
 * Vector.cpp
 *
 *  Created on: 28 de out de 2015
 *      Author: cassiano
 */

#include "Vector3.h"

namespace geometry {

Vector3::Vector3(GLfloat x, GLfloat y, GLfloat z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3::~Vector3() {
	// TODO Auto-generated destructor stub
}

Vector3 Vector3::operator+(Vector3 other) {
	return Vector3(x + other.getX(), y + other.getY(), z + other.getZ());
}

Vector3 Vector3::operator-(Vector3 other) {
	return Vector3(x - other.getX(), y - other.getY(), z - other.getZ());
}

GLfloat Vector3::operator*(Vector3 other) {
	return (x * other.getX()) + (y * other.getY()) + (z * other.getZ());
}

GLfloat Vector3::magnitude() {
	return (GLfloat) sqrtf( powf(x, 2.0f) + powf(y, 2.0f) + powf(z, 2.0f) );
}

void Vector3::normalize() {
	float magnitude = this->magnitude();

	this->x = this->x / magnitude;
	this->y = this->y / magnitude;
	this->z = this->z / magnitude;
}

GLfloat Vector3::getX() {
	return this->x;
}

GLfloat Vector3::getY() {
	return this->y;
}

GLfloat Vector3::getZ() {
	return this->z;
}

}
