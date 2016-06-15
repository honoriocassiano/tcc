/*
 * PerspectiveCamera2.cpp
 *
 *  Created on: 4 de mai de 2016
 *      Author: cassiano
 */

#include "PerspectiveCamera2.h"

#include <math.h>
#include "matrix.h"

#include <GL/gl.h>
#include <GL/glu.h>

#include <stdio.h>

PerspectiveCamera2::PerspectiveCamera2(Vec3f c, Vec3f d, Vec3f u, float a) :
		Camera(c, d, u) {
	angle = a;

	Vec3f screenCenter = center + direction;
	float screenHeight = tan(angle / 2.0);

	lowerLeft = screenCenter - (getScreenUp() * screenHeight)
			- (getHorizontal() * screenHeight);
	xAxis = getHorizontal() * 2 * screenHeight;
	yAxis = getScreenUp() * 2 * screenHeight;
}

// ====================================================================
// ====================================================================
// GL INIT
// Create a camera with the appropriate dimensions that
// crops the screen in the narrowest dimension.

void PerspectiveCamera2::glInit(int w, int h) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float aspect = float(w) / float(h);
	float asp_angle = angle * 180 / M_PI;
	if (aspect > 1)
		asp_angle /= aspect;

	gluPerspective(asp_angle, aspect, 1, 500.0);
}

// ====================================================================
// dollyCamera, truckCamera, and RotateCamera
//
// Asumptions:
//  - up is really up (i.e., it hasn't been changed
//    to point to "screen up")
//  - up and direction are normalized
// Special considerations:
//  - If your constructor precomputes any vectors for
//    use in 'generateRay', you will likely to recompute those
//    values at athe end of the these three routines
// ====================================================================

// ====================================================================
// dollyCamera: Move camera along the direction vector
// ====================================================================

void PerspectiveCamera2::dollyCamera(float dist) {
	center += direction * dist;
	Vec3f screenCenter = center + direction;
	float screenHeight = tan(angle / 2.0);
	lowerLeft = screenCenter - (getScreenUp() * screenHeight)
			- (getHorizontal() * screenHeight);
}

// ====================================================================
// truckCamera: Translate camera perpendicular to the direction vector
// ====================================================================

void PerspectiveCamera2::truckCamera(float dx, float dy) {
	center += getHorizontal() * dx + getScreenUp() * dy;
	Vec3f screenCenter = center + direction;
	float screenHeight = tan(angle / 2.0);
	lowerLeft = screenCenter - (getScreenUp() * screenHeight)
			- (getHorizontal() * screenHeight);
}

// ====================================================================
// rotateCamera: Rotate around the up and horizontal vectors
// ====================================================================

void PerspectiveCamera2::rotateCamera(float rx, float ry) {
	// Don't let the model flip upside-down (There is a singularity
	// at the poles when 'up' and 'direction' are aligned)
	float tiltAngle = acos(up.Dot3(direction));
	if (tiltAngle - ry > 3.13)
		ry = tiltAngle - 3.13;
	else if (tiltAngle - ry < 0.01)
		ry = tiltAngle - 0.01;

	Matrix rotMat = Matrix::MakeAxisRotation(up, rx);
	rotMat *= Matrix::MakeAxisRotation(getHorizontal(), ry);

//	printf("(%f, %f, %f) => ", direction.x(), direction.y(), direction.z());
	rotMat.TransformDirection(direction);
//	printf("(%f, %f, %f)\n", direction.x(), direction.y(), direction.z());

	direction.Normalize();
	Vec3f screenCenter = center + direction;
	float screenHeight = tan(angle / 2.0);

	lowerLeft = screenCenter - (getScreenUp() * screenHeight)
			- (getHorizontal() * screenHeight);
	xAxis = getHorizontal() * 2 * screenHeight;
	yAxis = getScreenUp() * 2 * screenHeight;
}
