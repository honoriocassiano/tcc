/*
 * PerspectiveCamera2.h
 *
 *  Created on: 4 de mai de 2016
 *      Author: cassiano
 */

#ifndef SRC_PERSPECTIVECAMERA_H_
#define SRC_PERSPECTIVECAMERA_H_

#include "structures/camera.h"

class PerspectiveCamera: public Camera {

public:
	// CONSTRUCTOR & DESTRUCTOR
	PerspectiveCamera(Vec3f c, Vec3f d, Vec3f u, float a);
	virtual ~PerspectiveCamera(void) {
	}

	const Vec3f& getPosition() const { return center; }

	// GL NAVIGATION
	void glInit(int w, int h) override;
	void dollyCamera(float dist);
	void truckCamera(float dx, float dy);
	void rotateCamera(float rx, float ry);
//	void Print() {
//		printf("PerspectiveCamera {\n");
//		printf("    center    ");
//		center.Write(stdout);
//		printf("    direction ");
//		direction.Write(stdout);
//		printf("    up        ");
//		up.Write(stdout);
//		printf("    angle      %f\n", angle);
//		printf("}\n");
//	}

	void lookAt(const Vec3f& point);

private:
	PerspectiveCamera() {
		assert(0);
	} // don't use

	// REPRESENTATION
	float angle;
	Vec3f lowerLeft;
	Vec3f xAxis;
	Vec3f yAxis;
};

#endif /* SRC_PERSPECTIVECAMERA_H_ */
