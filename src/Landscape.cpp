/*
 * Landscape.cpp
 *
 *  Created on: 20 de jul de 2016
 *      Author: cassiano
 */

#include "Landscape.h"

#include "Debug.h"

Landscape::Landscape() :
		mPatch(new Patch()) {

}

Landscape::~Landscape() {
	delete mPatch;
}

void Landscape::render() {

	mPatch->render();

	int numTriangles = mPatch->getMesh()->numTriangles();

	if (numTriangles <= maxTriangles) {
//		Log("%f.5\n", (numTriangles - (float)maxTriangles) / (float)maxTriangles);

		frameVariance +=  (numTriangles - (float)maxTriangles) / (float)maxTriangles;
	}

	if (frameVariance < 0) {
		frameVariance = 0;
	}
}

void Landscape::computeVariance() {
	mPatch->computeVariance();
}

void Landscape::toggleWireframe() {
	mPatch->toggleWireframe();
}

// TODO Check this method
void Landscape::tessellate(const Vec3f& cameraPosition) {

	Log("(%.5f, %.5f, %.5f)\n", cameraPosition.x(), cameraPosition.y(), cameraPosition.z());

	mPatch->tessellate(cameraPosition);
}
