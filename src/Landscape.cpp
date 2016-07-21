/*
 * Landscape.cpp
 *
 *  Created on: 20 de jul de 2016
 *      Author: cassiano
 */

#include "Landscape.h"

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
//		printf("%d, %lu, %e\n", numTriangles, maxTriangles, frameVariance);
		frameVariance +=  (numTriangles - (float)maxTriangles) / (float)maxTriangles;
	}

	if (frameVariance < 0) {
		frameVariance = 0;
	}
}

void Landscape::computeVariance() {
	mPatch->computeVariance();
}

// TODO Check this method
void Landscape::tessellate(const Vec3f& cameraPosition) {
	mPatch->tessellate(cameraPosition);
}
