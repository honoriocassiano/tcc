#include <math.h>
#include "camera.h"
#include "matrix.h"

#include <GL/gl.h>
#include <GL/glu.h>

#include <stdio.h>

// ====================================================================
// ====================================================================
// CONSTRUCTORS

Camera::Camera(Vec3f c, Vec3f d, Vec3f u) {
	center = c;
	direction = d;
	up = u;
	// normalize the vectors
	up.normalize();
	direction.normalize();
}

// ====================================================================
// ====================================================================
// GL PLACE CAMERA
// Place a camera within an OpenGL scene

void Camera::glPlaceCamera(void) {
	Vec3f lookAt = center + direction;
//	Vec3f lookAt = direction;
	gluLookAt(center.x(), center.y(), center.z(), lookAt.x(), lookAt.y(),
			lookAt.z(), up.x(), up.y(), up.z());

//  printf("(%f, %f, %f)\n", center.x(), center.y(), center.z());
	//printf("(%f, %f, %f) (%f, %f, %f)\n", lookAt.x(), lookAt.y(), lookAt.z(), lookAt2.x(), lookAt2.y(), lookAt2.z());
}

// ====================================================================
// ====================================================================
